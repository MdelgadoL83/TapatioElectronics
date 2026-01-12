/*
Código para el Seguidor de Línea Profesional Marca Tapatio Electrónics
Created by: Miguel Angel Delgado Lopez 
Contact: miguel.delgado.lop@gmail.com
Company: Tapatio Electronics
Date: Jan 2026
*/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "TapatioElectronics.h"
//REVISAR DEBUG
#define DEBUG false

//AJUSATR BATERIA :Deshabilitado por el momento
#define BATT false

//USAR CALIBRACION
#define CALIBRAR true

//SELECCIONA EL DRIVER
#define TB6612 
//#define DRV8870

//CHEQUEO DE MOTORES
#define TESTMOTORS false

//-----------------------------------SENSOR VARS------------------------------------------
int numSensores=16;
SensorBar16 sb(12,11,10,8,A2);       //(S0,S1,S2,S3,OUT)


//-----------------------------------POTS VARS------------------------------------------
int pots[]={A7,A6,A1,A0};


//-----------------------------------MOTOR VARS------------------------------------------
#ifdef TB6612
  /*
  NOTA:
  Invertir ain2 y ain1 si el motor A gira en sentido opuesto
  Invertir bin2 y bin1 si el motor B gira en sentido opuesto
  */
  int pwma = 3; //PWM
  int ain2 = 4; //Ain2 
  int ain1 = 5; //Ain1      
  int bin1 = 7; //Bin1
  int bin2 = 6; //Bin2
  int pwmb = 9; //PWM
#endif
#ifdef DRV8870
  /*
  NOTA:
  invertir pwma y ain1 si el motor A gira en sentido opuesto
  invertir pwmb y bin1 si el motor B gira en sentido opuesto
  */
  int pwma = 5; //PWM
  int ain2 = 4; //INPUT
  int ain1 = 3; //PWM
  int bin1 = 7; //PWM
  int bin2 = 6; //INPUT
  int pwmb = 9; //PWM
#endif

// ===================== PID mejorado (dt por micros, D filtrada, I anti-windup) =====================
// Estados del PID
float Iacc = 0.0f;     // Acumulador integral (∫ e·dt), limitado por I_MAX
float d_filt = 0.0f;   // Derivada filtrada (band-limited derivative)
float e_prev = 0.0f;   // Error previo para derivada
static unsigned long t_prev_us = 0; // Marca de tiempo previa en microsegundos (para dt)

// Límites y umbrales
const float U_MAX  = 255.0f; // Límite de |u|
const float I_MAX  = 200.0f; // Límite de |Iacc| (anti-windup)
const float E_I_ON = 80.0f;  // |e| debajo de este umbral -> sí integra (cerca de la línea)

// Derivada filtrada
float fc_d = 25.0f;                                // Frecuencia de corte del filtro D (Hz) [20..35]
float tau_d = 1.0f / (6.2831853f * 25.0f);         // tau = 1/(2π·fc). Recalcular en setup si cambias fc_d.

// Freno dinámico (sobre velocidad base)
float kf    = 0.0f;   // Fuerza del freno dinámico (10..16 típico)
float scale = 16.0f;   // Escala para normalizar e en el término cuadrático (e/scale)^2

int vel=40;

//-----------------------------------PID VARS------------------------------------------
//NOTA: es tos valores seran sobreescritos con los potenciometros
float kp=.3;
float ki=0.000001;
float kd=0.0;
float P,I,D;
float error =0;

//-----------------------------------BUTTON & BATTERY VARS------------------------------------------
//NOTA: por en momento no se usa, pero siente libre de implementar el coltimetro
boolean start = false;
long time=0;
#define batt A3
#define boton 2

//-----------------------------------SCREEN VARS---------------------------------------------------
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);
String cad;



void setup() {
  // Recalcular constante de tiempo del filtro derivativo
  tau_d = 1.0f / (6.2831853f * fc_d);
  // Inicializar marca de tiempo para dt
  t_prev_us = micros();


   // Configurar timers para PWM a ~976Hz (prescaler 64)
  TCCR1B = TCCR1B & B11111000 | B00000011; // Timer1: D9 y D10, 16MHz/(64*256)=976Hz
  TCCR2B = TCCR2B & B11111000 | B00000100; // Timer2: D3 y D11, 16MHz/(64*256)=976Hz
  
  Serial.begin(115200);
  pinMode(LED_BUILTIN,OUTPUT);

 //-----------------------------------SCREEN------------------------------------------
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  SplashScreen(display);
  delay(3000);
  display.clearDisplay();
  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  
//-----------------------------------DRIVER TB6612------------------------------------------
  #ifdef TB6612
    pinMode(pwma,OUTPUT);
    pinMode(ain2,OUTPUT);
    pinMode(ain1,OUTPUT);
    pinMode(bin1,OUTPUT);
    pinMode(bin2,OUTPUT);
    pinMode(pwmb,OUTPUT);
  #endif
//-----------------------------------DRIVER DRV8870------------------------------------------
  #ifdef DRV8870
    pinMode(pwma,OUTPUT);//3
    pinMode(ain2,INPUT);//4
    pinMode(ain1,OUTPUT);//5
    pinMode(bin1,OUTPUT);//6
    pinMode(bin2,INPUT);//7
    pinMode(pwmb,OUTPUT);//9
  #endif

  #if TESTMOTORS
    while(true){
      #ifdef TB6612
        velocidad(40,40);
      #else
        velocidadDRV(40,40);
      #endif
    }
  #endif

//-----------------------------------POTENCIOMETROS------------------------------------------
  for(int x=0;x<4;x++){
    pinMode(pots[x],INPUT);
  }

//--------------------------------------------BOTON------------------------------------------
  pinMode(boton,INPUT_PULLUP);

  //-----------------------------------------BATERIA------------------------------------------
  pinMode(batt,INPUT);

  display.setCursor(10,15);
  display.print("C A L I B R A N D O");
  display.display();

  #if CALIBRAR                          //white black white cycles through each sensor to set the detection thresholdby,
  sb.calibrate();                //default the process is done 800 times, but you can specify the value you need 
  #endif                               //inside the parentheses               
  
  velocidad(0,0);
  time=millis();

}

void loop() {

  
  if(digitalRead(boton)==LOW){
    start=!start;
    velocidad(0,0);
    delay(500);
    if(start)
      comenzar(3);                   //Tiempo de espera al presionar el boton de inicio
  }

  if(!start){
    pantalla();
    velocidad(0,0);
    

  }else{

    // ======================== Control PID mejorado ============================
    // 1) Calcular dt
    unsigned long t_now_us = micros();
    float dt = (t_now_us - t_prev_us) * 1e-6f; // segundos
    t_prev_us = t_now_us;
    if (dt < 0.0005f) dt = 0.0005f;            // 0.5 ms mínimo (evita dt~0)
    if (dt > 0.02f)   dt = 0.02f;              // 20 ms máximo (evita saltos por bloqueos)

    // 2) Leer posición y definir error (setpoint = 0, línea centrada)
    int pos = sb.getPosition(255);             // ≈ [-255..255]
    pos = constrain(pos, -255, 255);
    float e = (float)pos;

    // 3) Derivada filtrada (band-limited)
    float d_raw   = (e - e_prev) / dt;         // de/dt correcta
    float alpha_d = dt / (tau_d + dt);         // 0..1
    d_filt += alpha_d * (d_raw - d_filt);      // filtro pasa-bajas 1er orden
    e_prev  = e;

    // 4) Integral con anti-windup (integración condicionada + límite + fuga)
    bool enable_I = (fabs(e) < E_I_ON);        // integra cerca de la línea
    if (enable_I) {
      Iacc += e * dt;                          // ∫ e·dt
      if (Iacc >  I_MAX) Iacc =  I_MAX;
      if (Iacc < -I_MAX) Iacc = -I_MAX;
    } else {
      Iacc *= 0.995f;                          // fuga suave (leaky integrator)
    }

    // 5) PID y saturación de corrección (u)
    // Nota: se usan las ganancias existentes 'kp','ki','kd' obtenidas de los potenciometros
    float u_unsat = kp*e + ki*Iacc + kd*d_filt;
    float u = u_unsat;
    if (u >  U_MAX) u =  U_MAX;
    if (u < -U_MAX) u = -U_MAX;

    // 6) Freno dinámico sobre la velocidad base (desacoplado del PID)(si kf es 0 no tiene efecto)
    float base = vel - abs(kf * e);    // recorte NO cuadrático


    // 7) Publicar variables compatibles con el resto del código
    int pid    = (int)round(u);                    // La variable 'pid' ahora representa la corrección saturada
    int velFin = (int)round(base);                 // La variable 'velFin' es la base dinámica
    
    //Si debug esta activo se muestran los valores en consola pero no se aplican a los motores
    #if DEBUG
      Serial.print("E= ");
      Serial.print(e);
      Serial.print(" PID= ");
      Serial.print(pid);
      Serial.print(" Vel= ");
      Serial.print(velFin);
      Serial.print(" IZQ= ");
      Serial.print(velFin+pid);
      Serial.print(" DER= ");
      Serial.print(velFin-pid);
      Serial.println();
      #else
        #ifdef TB6612
          velocidad(velFin+pid,velFin-pid);
        #else
          velocidadDRV(velFin+pid,velFin-pid);
        #endif
      #endif
  }

}

/*
Metodo para controlar la velocidad si se usa el driver Tapatio DRV8870
el cual se controla con 2 pwm por motor
*/
void velocidadDRV(int izq, int der){
  if (izq>0){
    analogWrite(pwma,izq);
    analogWrite(ain1,0);
  }else{
    analogWrite(pwma,5);
    analogWrite(ain1,-izq);
  }
  if (der>0){
    analogWrite(pwmb,der);
    analogWrite(bin1,0);
  }else{
    analogWrite(pwmb,5);
    analogWrite(bin1,-der);
  }
}

/*
Metedo utilizado si se usa el tradicional Driver TB6612
que usa un PWM e in1, in2 para controlar velocidad y sentido
*/
void velocidad(int izq, int der){
  if(izq>255){
    izq=255;
  }
  if(der>255){
    der=255;
  }
  if(izq<-255){
    izq=-255;
  }
  if(der<-255){
    der=-255;
  }
  if(izq>=0){
    digitalWrite(ain2, HIGH);
    digitalWrite(ain1, LOW);
  }else{
    digitalWrite(ain2, LOW);
    digitalWrite(ain1, HIGH);
    izq=abs(izq);
  }
  if(der>=0){
    digitalWrite(bin2, HIGH);
    digitalWrite(bin1, LOW);
  }else{
    digitalWrite(bin2, LOW);
    digitalWrite(bin1, HIGH);
    der=abs(der);
  }
  analogWrite(pwma,izq);
  analogWrite(pwmb,der);
}

/*
Este método controla todo lo que sucede en la pantalla, además es el
encargado de actualizar las variables kp, ki y kd provenientes de
los potenciometros
*/
void pantalla(){
    String cad="";
    int* valores=sb.digitalValues(); //Call the method to read values 
    for(int x=0;x<numSensores;x++){
      cad+=valores[x]!=0?"0":"_";
    }
    display.clearDisplay();
    display.setCursor(5, 0);
    display.print(" P = ");
    int p1=analogRead(pots[0]);
    kp=p1*.002;
    display.println(kp);
    display.setCursor(65, 0);
    display.print(" I = ");
    int p2=analogRead(pots[1]);
    ki=p2*.002;
    display.println(ki);
    display.setCursor(5, 10);
    display.print(" D = ");
    int p3=analogRead(pots[2]);
    kd=p3*.002;
    display.println(kd);
    display.setCursor(65, 10);
    display.print(" V = ");
    int p4=analogRead(pots[3]);
    vel=map(p4,0,1024,0,255);
    display.println(vel);
    //IMPRIME LA LINEA DE SENSORES
    display.setCursor(15, 25);
    display.print(cad);
    display.display();

    /*
    TODO: mostrar el nivel de bateria
    display.fillRect(121,6,3,2,SSD1306_WHITE);//top
    display.drawLine(117,8,127,8,SSD1306_WHITE);//up
    display.drawLine(117,30,127,30,SSD1306_WHITE);//bottom
    display.drawLine(117,8,117,30,SSD1306_WHITE);//right
    display.drawLine(127,8,127,30,SSD1306_WHITE);//left

    display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
   
    
    if(bp>80)display.fillRect(119,26,7,3,SSD1306_WHITE);
    if(bp>84)display.fillRect(119,22,7,3,SSD1306_WHITE);
    if(bp>88)display.fillRect(119,18,7,3,SSD1306_WHITE);
    if(bp>92)display.fillRect(119,14,7,3,SSD1306_WHITE);
    if(bp>96)display.fillRect(119,10,7,3,SSD1306_WHITE);
    */

 
}

void comenzar(int r){
  r=r>0?r:5;
  if(start){
    for(int x=r;x>0;x--){
      display.clearDisplay();
      display.setCursor(25,10);
      display.print("INICIANDO EN ");
      display.setCursor(60,20);
      display.print(x);
      display.display();
      delay(1000);
    }
    display.clearDisplay();
    display.setCursor(15,15);
    display.print("C O R R I E N D O");
    display.display();
  }
}

