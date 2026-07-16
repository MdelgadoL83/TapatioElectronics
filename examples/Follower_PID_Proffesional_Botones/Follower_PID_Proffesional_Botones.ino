/*
Código para el Seguidor de Línea Profesional Marca Tapatio Electrónics
Created by: Miguel Angel Delgado Lopez 
Contact: miguel.delgado.lop@gmail.com
Company: Tapatio Electronics
Modified for Buttons Navigation, EEPROM Storage, Anti-Wear Timeout & Tuning Section
Date: 2026
*/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <EEPROM.h> 
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

/// ======================================================================================
// ----------------------- CONFIGURATION & TUNING VARS ----------------------------------
// ======================================================================================
// Modifica estos valores para ajustar la sensibilidad de los botones y guardado:

const float PASO_CORTO = 0.01f;         // Incremento/decremento al dar un toque rápido (PID)
const float PASO_LARGO = .10f;         // Incremento/decremento en modo ráfaga/mantenido (PID)
const int PASO_VEL     = 5;            // Incremento/decremento para la velocidad base (V)

const int TIEMPO_DETECCION_LARGO = 600; // Milisegundos a presionar antes de activar ráfaga
const int VELOCIDAD_RAFAGA       = 80; // Milisegundos entre cada cambio en modo ráfaga

const unsigned long TIEMPO_ESPERA_EEPROM = 5000; // Tiempo de inactividad (ms) antes de salvar en EEPROM



// --------------------------------------------------------------------------------------

//-----------------------------------SENSOR VARS------------------------------------------
int numSensores=16;
SensorBar16 sb(12,11,10,8,A6);       

//-----------------------------------BUTTONS VARS------------------------------------------
const int btnDerecha   = A0; 
const int btnIzquierda = A1;
const int btnDown      = A2;
const int btnUp        = A3;

int indiceSeleccionado = 0; 
int pantallaActual = 0;  // 0 = pantalla PID/Vel, 1 = pantalla Kf

//-----------------------------------MOTOR VARS------------------------------------------
#ifdef TB6612
  int pwma = 3; 
  int ain2 = 5; 
  int ain1 = 4;       
  int bin1 = 6; 
  int bin2 = 7; 
  int pwmb = 9; 
#endif
#ifdef DRV8870
  int pwma = 5; 
  int ain2 = 4; 
  int ain1 = 3; 
  int bin1 = 7; 
  int bin2 = 6; 
  int pwmb = 9; 
#endif

// ===================== PID mejorado =====================
float Iacc = 0.0f;     
float d_filt = 0.0f;   
float e_prev = 0.0f;   
static unsigned long t_prev_us = 0; 

const float U_MAX  = 255.0f; 
const float I_MAX  = 200.0f; 
const float E_I_ON = 80.0f;  

float fc_d = 25.0f;                                
float tau_d = 1.0f / (6.2831853f * 25.0f);         

float scale = 16.0f;   

int vel = 40;

//-----------------------------------PID VARS------------------------------------------
float kp = 0.70;
float ki = 0.10;
float kd = 0.02;
float kf = 0.3f;   
float P, I, D;
float error = 0;

//-----------------------------------EEPROM ADDRESSES------------------------------------
const int addrKp  = 0;
const int addrKi  = 4;
const int addrKd  = 8;
const int addrVel = 12;
const int addrkf =16; 

//-----------------------------------TIMERS PARA EEPROM----------------------------------
unsigned long ultimoCambioTiempo = 0;  
boolean pendienteDeGuardar = false;    

//-----------------------------------START BUTTON------------------------------------------
boolean start = false;
long time = 0;
#define boton 2

//-----------------------------------START BUTTON------------------------------------------
#define ir_control A7

//-----------------------------------SCREEN VARS---------------------------------------------------
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 32 
#define SCREEN_ADDRESS 0x3C 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);

void setup() {
  tau_d = 1.0f / (6.2831853f * fc_d);
  t_prev_us = micros();

  TCCR1B = TCCR1B & B11111000 | B00000011; 
  TCCR2B = TCCR2B & B11111000 | B00000100; 
  
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);

  EEPROM.get(addrKp, kp);
  EEPROM.get(addrKi, ki);
  EEPROM.get(addrKd, kd);
  EEPROM.get(addrVel, vel);
  EEPROM.get(addrkf, kf);

  if (isnan(kp) || kp < 0) kp = 0.3f;
  if (isnan(ki) || ki < 0) ki = 0.000001f;
  if (isnan(kd) || kd < 0) kd = 0.0f;
  if (vel < 0 || vel > 255) vel = 40;
  if (isnan(kf) || kf < 0) kf = 1.0f;

  //-----------------------------------SCREEN------------------------------------------
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  SplashScreen(display);
  delay(3000);
  display.clearDisplay();
  display.setTextSize(1);      
  display.setTextColor(SSD1306_WHITE); 
  
  //-----------------------------------DRIVER------------------------------------------
  #ifdef TB6612
    pinMode(pwma, OUTPUT);
    pinMode(ain2, OUTPUT);
    pinMode(ain1, OUTPUT);
    pinMode(bin1, OUTPUT);
    pinMode(bin2, OUTPUT);
    pinMode(pwmb, OUTPUT);
  #endif
  #ifdef DRV8870
    pinMode(pwma, OUTPUT);
    pinMode(ain2, INPUT);
    pinMode(ain1, OUTPUT);
    pinMode(bin1, OUTPUT);
    pinMode(bin2, INPUT);
    pinMode(pwmb, OUTPUT);
  #endif

  #if TESTMOTORS
    while(true){
      #ifdef TB6612
        velocidad(40, 40);
      #else
        velocidadDRV(40, 40);
      #endif
    }
  #endif

  pinMode(btnDerecha, INPUT_PULLUP);
  pinMode(btnIzquierda, INPUT_PULLUP);
  pinMode(btnDown, INPUT_PULLUP);
  pinMode(btnUp, INPUT_PULLUP);
  pinMode(boton, INPUT_PULLUP);

  pinMode(ir_control, INPUT);
  display.setCursor(10, 15);
  display.print("C A L I B R A N D O");
  display.display();

  #if CALIBRAR                          
    sb.calibrate();                
  #endif                               
  
  velocidad(0, 0);
  time = millis();
}

void loop() {

  bool ir=analogRead(ir_control)>600;

  if(digitalRead(boton) == LOW && !ir){
    if(pendienteDeGuardar) {
      actualizarEEPROM();
    }
    start = !start;
    velocidad(0, 0);
    delay(500);
    if(start)
      comenzar(3);                   
  }

  if(!start && !ir){
    leerBotonesMenu(); 
    verificarGuardadoEEPROM(); 
    pantalla();
    velocidad(0, 0);
  } else {
    // ======================== Control PID mejorado ============================
    unsigned long t_now_us = micros();
    float dt = (t_now_us - t_prev_us) * 1e-6f; 
    t_prev_us = t_now_us;
    if (dt < 0.0005f) dt = 0.0005f;            
    if (dt > 0.02f)   dt = 0.02f;              

    int pos = sb.getPosition(255);             
    pos = constrain(pos, -255, 255);
    float e = (float)pos;

    float d_raw   = (e - e_prev) / dt;         
    float alpha_d = dt / (tau_d + dt);         
    d_filt += alpha_d * (d_raw - d_filt);      
    e_prev  = e;

    bool enable_I = (fabs(e) < E_I_ON);        
    if (enable_I) {
      Iacc += e * dt;                          
      if (Iacc >  I_MAX) Iacc =  I_MAX;
      if (Iacc < -I_MAX) Iacc = -I_MAX;
    } else {
      Iacc *= 0.995f;                          
    }

    float u_unsat = kp*e + ki*Iacc + kd*d_filt;
    float u = u_unsat;
    if (u >  U_MAX) u =  U_MAX;
    if (u < -U_MAX) u = -U_MAX;

    float pot = e/10;
    pot = pot*pot;
    float base = vel - abs(kf *pot);    

    int pid    = (int)round(u);                    
    int velFin = (int)round(base);                 
    
    #if DEBUG
      Serial.print("E= "); Serial.print(e);
      Serial.print(" PID= "); Serial.print(pid);
      Serial.print(" Vel= "); Serial.print(velFin);
      Serial.println();
    #else
      #ifdef TB6612
        velocidad(velFin+pid, velFin-pid);
      #else
        velocidadDRV(velFin+pid, velFin-pid);
      #endif
    #endif
  }
}

void leerBotonesMenu() {
  // --- BOTONES DERECHA/IZQUIERDA (NAVEGACIÓN SECUENCIAL) ---
  if (digitalRead(btnDerecha) == LOW) {
    // Si estamos en pantalla 0 y en el último elemento (vel), pasar a pantalla 1 (kf)
    if (pantallaActual == 0 && indiceSeleccionado == 3) {
      pantallaActual = 1;
      indiceSeleccionado = 4;  // kf
    }
    // Si estamos en pantalla 1 y en kf, volver a pantalla 0 (kp)
    else if (pantallaActual == 1 && indiceSeleccionado == 4) {
      pantallaActual = 0;
      indiceSeleccionado = 0;  // kp
    }
    // En cualquier otro caso, solo avanzar al siguiente índice
    else {
      indiceSeleccionado++;
    }
    delay(200); 
  }
  
  if (digitalRead(btnIzquierda) == LOW) {
    // Si estamos en pantalla 0 y en el primer elemento (kp), ir a pantalla 1 (kf)
    if (pantallaActual == 0 && indiceSeleccionado == 0) {
      pantallaActual = 1;
      indiceSeleccionado = 4;  // kf
    }
    // Si estamos en pantalla 1 y en kf, volver a pantalla 0 (vel)
    else if (pantallaActual == 1 && indiceSeleccionado == 4) {
      pantallaActual = 0;
      indiceSeleccionado = 3;  // vel
    }
    // En cualquier otro caso, solo retroceder al índice anterior
    else {
      indiceSeleccionado--;
    }
    delay(200);
  }

  // --- BOTÓN UP (INCREMENTAR) ---
  if (digitalRead(btnUp) == LOW) {
    unsigned long tiempoPresionado = millis();
    boolean primerToque = true;

    while (digitalRead(btnUp) == LOW) {
      unsigned long duracion = millis() - tiempoPresionado;

      if (primerToque) {
        if (indiceSeleccionado == 0) kp += PASO_CORTO;
        if (indiceSeleccionado == 1) ki += PASO_CORTO;
        if (indiceSeleccionado == 2) kd += PASO_CORTO;
        if (indiceSeleccionado == 3) { vel += PASO_VEL; if (vel > 255) vel = 255; }
        if (indiceSeleccionado == 4) kf += PASO_CORTO;
        primerToque = false;
        pantalla(); 
        delay(200); 
      } 
      else if (duracion > TIEMPO_DETECCION_LARGO && indiceSeleccionado != 3 && indiceSeleccionado != 4) {
        if (indiceSeleccionado == 0) kp += PASO_LARGO;
        if (indiceSeleccionado == 1) ki += PASO_LARGO;
        if (indiceSeleccionado == 2) kd += PASO_LARGO;
        if (indiceSeleccionado == 4) kf += PASO_LARGO;
        pantalla(); 
        delay(VELOCIDAD_RAFAGA); 
      }
    }
    ultimoCambioTiempo = millis();
    pendienteDeGuardar = true;
  }

  // --- BOTÓN DOWN (DECREMENTAR) ---
  if (digitalRead(btnDown) == LOW) {
    unsigned long tiempoPresionado = millis();
    boolean primerToque = true;

    while (digitalRead(btnDown) == LOW) {
      unsigned long duracion = millis() - tiempoPresionado;

      if (primerToque) {
        if (indiceSeleccionado == 0) { kp -= PASO_CORTO; if(kp < 0) kp = 0; }
        if (indiceSeleccionado == 1) { ki -= PASO_CORTO; if(ki < 0) ki = 0; }
        if (indiceSeleccionado == 2) { kd -= PASO_CORTO; if(kd < 0) kd = 0; }
        if (indiceSeleccionado == 3) { vel -= PASO_VEL; if (vel < 0) vel = 0; }
        if (indiceSeleccionado == 4) { kf -= PASO_CORTO; if(kf < 0) kf = 0; }
        primerToque = false;
        pantalla();
        delay(200);
      } 
      else if (duracion > TIEMPO_DETECCION_LARGO && indiceSeleccionado != 3 && indiceSeleccionado != 4) {
        if (indiceSeleccionado == 0) kp -= PASO_LARGO;
        if (indiceSeleccionado == 1) ki -= PASO_LARGO;
        if (indiceSeleccionado == 2) kd -= PASO_LARGO;
        if (indiceSeleccionado == 4) kf -= PASO_LARGO;
        
        if (kp < 0) kp = 0;
        if (ki < 0) ki = 0;
        if (kd < 0) kd = 0;
        if (kf < 0) kf = 0;
        
        pantalla();
        delay(VELOCIDAD_RAFAGA);
      }
    }
    ultimoCambioTiempo = millis();
    pendienteDeGuardar = true;
  }
}

void verificarGuardadoEEPROM() {
  if (pendienteDeGuardar && (millis() - ultimoCambioTiempo >= TIEMPO_ESPERA_EEPROM)) {
    actualizarEEPROM();
  }
}

void actualizarEEPROM() {
  EEPROM.put(addrKp, kp);
  EEPROM.put(addrKi, ki);
  EEPROM.put(addrKd, kd);
  EEPROM.put(addrVel, vel);
  EEPROM.put(addrkf, kf);
  pendienteDeGuardar = false;
  
  digitalWrite(LED_BUILTIN, HIGH);
  delay(50);
  digitalWrite(LED_BUILTIN, LOW);
}

void pantalla(){
    String cad="";
    int* valores=sb.digitalValues(); 
    for(int x=0;x<numSensores;x++){
      cad+=valores[x]!=0?"0":"_";
    }
    display.clearDisplay();
    
    // Indicador discreto de guardado pendiente en la EEPROM
    if (pendienteDeGuardar) {
      display.setCursor(122, 0);
      display.setTextColor(SSD1306_WHITE);
      display.print(".");
    }

    // -------------------------------------------------------------------------
    // PANTALLA 0: Kp, Ki, Kd, Vel
    // -------------------------------------------------------------------------
    if (pantallaActual == 0) {
      // FILA 1: P (Kp) y I (Ki)
      
      // --- Caja de Kp ---
      if(indiceSeleccionado == 0) {
        display.fillRect(1, 0, 54, 11, SSD1306_WHITE);
        display.setTextColor(SSD1306_BLACK);
      } else {
        display.setTextColor(SSD1306_WHITE);
      }
      display.setCursor(3, 2);
      display.print(" P= "); display.print(kp, 2);

      // --- Caja de Ki ---
      if(indiceSeleccionado == 1) {
        display.fillRect(61, 0, 58, 11, SSD1306_WHITE);
        display.setTextColor(SSD1306_BLACK);
      } else {
        display.setTextColor(SSD1306_WHITE);
      }
      display.setCursor(63, 2);
      display.print("I = "); display.print(ki, 2);

      // FILA 2: D (Kd) y V (Vel)
      
      // --- Caja de Kd ---
      if(indiceSeleccionado == 2) {
        display.fillRect(1, 11, 54, 11, SSD1306_WHITE);
        display.setTextColor(SSD1306_BLACK);
      } else {
        display.setTextColor(SSD1306_WHITE);
      }
      display.setCursor(3, 13);
      display.print(" D= "); display.print(kd, 2);

      // --- Caja de Velocidad ---
      if(indiceSeleccionado == 3) {
        display.fillRect(61, 11, 48, 11, SSD1306_WHITE);
        display.setTextColor(SSD1306_BLACK);
      } else {
        display.setTextColor(SSD1306_WHITE);
      }
      display.setCursor(63, 13);
      display.print("V= "); display.print(vel);
    }
    
    // -------------------------------------------------------------------------
    // PANTALLA 1: Kf
    // -------------------------------------------------------------------------
    else if (pantallaActual == 1) {
      display.setCursor(9,0);
      display.print(" FRENO DINAMICO ");
      // --- Caja de Kf (centrada en la pantalla) ---
      if(indiceSeleccionado == 4) {
        display.fillRect(24, 10, 60, 12, SSD1306_WHITE);
        display.setTextColor(SSD1306_BLACK);
      } else {
        display.setTextColor(SSD1306_WHITE);
      }
      display.setCursor(25, 12);
      display.print(" KF= "); display.print(kf, 2);
    }

    // -------------------------------------------------------------------------
    // FILA 3: LINEA DE SENSORES
    // -------------------------------------------------------------------------
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(15, 25);
    display.print(cad);
    
    display.display();
}

void velocidadDRV(int izq, int der){
  if (izq>0){analogWrite(pwma,izq); analogWrite(ain1,0);}
  else{analogWrite(pwma,5); analogWrite(ain1,-izq);}
  if (der>0){analogWrite(pwmb,der); analogWrite(bin1,0);}
  else{analogWrite(pwmb,5); analogWrite(bin1,-der);}
}

void velocidad(int izq, int der){
  if(izq>255) izq=255; if(der>255) der=255;
  if(izq<-255) izq=-255; if(der<-255) der=-255;
  if(izq>=0){digitalWrite(ain2, HIGH); digitalWrite(ain1, LOW);}
  else{digitalWrite(ain2, LOW); digitalWrite(ain1, HIGH); izq=abs(izq);}
  if(der>=0){digitalWrite(bin2, HIGH); digitalWrite(bin1, LOW);}
  else{digitalWrite(bin2, LOW); digitalWrite(bin1, HIGH); der=abs(der);}
  analogWrite(pwma,izq); analogWrite(pwmb,der);
}

void comenzar(int r){
  r=r>0?r:5;
  if(start){
    for(int x=r;x>0;x--){
      display.clearDisplay(); display.setCursor(25,10); display.print("INICIANDO EN ");
      display.setCursor(60,20); display.print(x); display.display(); delay(1000);
    }
    display.clearDisplay(); display.setCursor(15,15); display.print("C O R R I E N D O"); display.display();
  }
}