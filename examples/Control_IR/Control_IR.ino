/*
 * TRANSMISOR IR - Control de Juez para Robotica de la marca Tapatio Electronnics
 * Microcontrolador: ATtiny412
 * Protocolo: Sony SIRC (12-bit) a 40 kHz
 * Envía el comando 3 veces para máxima seguridad.
 * Created by: Miguel Angel Delgado Lopez
 * Contact:    miguel.delgado.lop@gmail.com
 * Company:    Tapatio Electronics
 * Date:       April 2022
*/


#include "config.h" // <--- CARGA TU CONFIGURACIÓN MODIFICANDO ESTE ARCHIVO

// --- ESTADOS DEL SISTEMA ---
enum SystemState { RESET, READY, GO };
SystemState currentState = RESET;

// --- VARIABLES DE TIEMPO --- 
unsigned long lastActivityTime = 0; // Registra la última vez que se presionó un botón o parpadeó

void setup() {
  // Usar los defines del config.h
  pinMode(TX_IR_SEND_PIN, OUTPUT);
  pinMode(TX_BTN_RESET, INPUT_PULLUP);
  pinMode(TX_BTN_RDY, INPUT_PULLUP);
  pinMode(TX_BTN_GO, INPUT_PULLUP);
  pinMode(TX_LEDS, OUTPUT);
  
  digitalWrite(TX_LEDS, LOW);
  digitalWrite(TX_IR_SEND_PIN, LOW);

  // Secuencia de inicio visual
  led_Flash(50);
  delay(50);
  led_Flash(50);
  delay(50);
  led_Flash(50);

  // Inicializar el contador de tiempo
  lastActivityTime = millis();
}

void loop() {
  // --- LÓGICA DE BOTONES Y ESTADOS ---
  if (digitalRead(TX_BTN_RESET) == LOW) {
    currentState = RESET;
    sendCommandSecure(IR_ADDRESS, CMD_RESET);
    led_Flash(100);
    while(digitalRead(TX_BTN_RESET) == LOW); // Anti-rebote
    lastActivityTime = millis(); // Reiniciar contador de inactividad
  }
  else if (digitalRead(TX_BTN_RDY) == LOW && currentState == RESET) {
    currentState = READY;
    sendCommandSecure(IR_ADDRESS, CMD_RDY);
    led_Flash(100);
    while(digitalRead(TX_BTN_RDY) == LOW);
    lastActivityTime = millis(); // Reiniciar contador de inactividad
  }
  else if (digitalRead(TX_BTN_GO) == LOW && currentState == READY) {
    currentState = GO;
    sendCommandSecure(IR_ADDRESS, CMD_GO);
    led_Flash(100);
    while(digitalRead(TX_BTN_GO) == LOW);
    lastActivityTime = millis(); // Reiniciar contador de inactividad
  }

  // --- LÓGICA DE INACTIVIDAD (Latido / Heartbeat) ---
  // Si el tiempo transcurrido es mayor al configurado en config.h
  if ((millis() - lastActivityTime) > TX_IDLE_TIMEOUT_MS) {
      lastActivityTime = millis(); // Reiniciar para que vuelva a pasar a los 5s
      led_Flash(TX_IDLE_FLASH_MS); // Parpadeo rápido usando el tiempo configurado
  }
}

// --- FUNCIÓN DE ENVÍO SEGURO (3 veces) ---
void sendCommandSecure(uint8_t address, uint8_t command) {
  for (int i = 0; i < 3; i++) {
    sendSony(address, command);
    if (i < 2) { 
      delay(45); 
    }
  }
}

// --- FUNCIÓN DE PARPADEO ---
void led_Flash(long onTime) {
  digitalWrite(TX_LEDS, HIGH);
  delay(onTime);
  digitalWrite(TX_LEDS, LOW);
}

// --- GENERADOR DE PORTADORA 40 kHz ---
void sendSonyPulse(unsigned long pulseLenUs) {
  unsigned long start = micros();
  while (micros() - start < pulseLenUs) {
    digitalWriteFast(TX_IR_SEND_PIN, HIGH); 
    delayMicroseconds(12);               
    digitalWriteFast(TX_IR_SEND_PIN, LOW);  
    delayMicroseconds(13);               
  }
}

// --- ESTRUCTURA DEL PROTOCOLO SONY SIRC ---
void sendSony(uint8_t address, uint8_t command) {
  sendSonyPulse(SONY_HDR_MARK);
  delayMicroseconds(600);
  
  uint16_t data = (address << 7) | command;
  
  for (int i = 0; i < 12; i++) {
    if (data & (1 << i)) {
      sendSonyPulse(SONY_BIT_ONE); 
    } else {
      sendSonyPulse(SONY_BIT_ZERO);  
    }
    delayMicroseconds(600); 
  }
}