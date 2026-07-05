/*
 * RECEPTOR IR - Módulo de Arranque MARCA TAPATIO ELECTRONICS
 * Este cogigo utliza protogolo Sony de 12 bits utilizado en los comntroles de la marca Tapatio Electronics
 * Created by: Miguel Angel Delgado Lopez
 * Contact:    miguel.delgado.lop@gmail.com
 * Company:    Tapatio Electronics
 * Date:       April 2022
 */

#include "config.h" // <--- CARGA TU CONFIGURACIÓN MODIFICANDO ESTE ARCHIVO

enum IrState { STATE_IDLE, STATE_DECODING };

volatile IrState state = STATE_IDLE;
volatile uint16_t shift_reg = 0;
volatile uint8_t bit_count = 0;
volatile uint32_t mark_start_time = 0;

volatile uint8_t raw_command = 0xFF;
volatile bool new_ir_frame = false;

void setup() {
  pinMode(RX_IR_PIN, INPUT_PULLUP);
  pinMode(RX_PIN_RDY, OUTPUT);
  pinMode(RX_PIN_GO, OUTPUT);

  digitalWrite(RX_PIN_RDY, LOW);
  digitalWrite(RX_PIN_GO, LOW);

  attachInterrupt(digitalPinToInterrupt(RX_IR_PIN), ir_isr, CHANGE);
}

void loop() {
  if (new_ir_frame) {
    noInterrupts();
    uint8_t cmd = raw_command;
    new_ir_frame = false;
    interrupts();

    if (cmd == CMD_RDY) {
      digitalWrite(RX_PIN_RDY, HIGH);
      digitalWrite(RX_PIN_GO, LOW);
    } 
    else if (cmd == CMD_GO) {
      digitalWrite(RX_PIN_GO, HIGH);
      digitalWrite(RX_PIN_RDY, LOW);
    } 
    else if (cmd == CMD_RESET) {
      digitalWrite(RX_PIN_RDY, LOW);
      digitalWrite(RX_PIN_GO, LOW);
    }
  }
}

void ir_isr() {
  bool is_low = !digitalReadFast(RX_IR_PIN); 

  if (is_low) {
    mark_start_time = micros();
  } 
  else {
    uint32_t duration = micros() - mark_start_time;

    if (state == STATE_IDLE) {
      if (duration >= (SONY_HDR_MARK - TOLERANCIA) && duration <= (SONY_HDR_MARK + TOLERANCIA)) {
        state = STATE_DECODING;
        bit_count = 0;
        shift_reg = 0;
      }
    } 
    else if (state == STATE_DECODING) {
      bool bit_val = false;

      if (duration >= (SONY_BIT_ONE - TOLERANCIA) && duration <= (SONY_BIT_ONE + TOLERANCIA)) {
        bit_val = true;
      } 
      else if (duration >= (SONY_BIT_ZERO - TOLERANCIA) && duration <= (SONY_BIT_ZERO + TOLERANCIA)) {
        bit_val = false;
      } 
      else {
        state = STATE_IDLE;
        return;
      }

      shift_reg |= ((uint16_t)bit_val << bit_count);
      bit_count++;

      if (bit_count == 12) {
        
        // --- EXTRAER DATOS DEL PAQUETO DE 12 BITS ---
        // Comando: Los primeros 7 bits (máscara 0x7F)
        uint8_t received_cmd = shift_reg & 0x7F;
        // Dirección: Los últimos 5 bits (corremos 7 espacios a la derecha y enmascaramos 0x1F)
        uint8_t received_addr = (shift_reg >> 7) & 0x1F;

        // --- LÓGICA DE VALIDACIÓN CONFIGURABLE ---
        if (RX_IGNORE_ADDRESS) {
          // Modo Abierto: Acepta el comando sin importar la dirección
          raw_command = received_cmd;
          new_ir_frame = true;
        } 
        else {
          // Modo Estricto: Solo acepta si la dirección coincide con IR_ADDRESS
          if (received_addr == IR_ADDRESS) {
            raw_command = received_cmd;
            new_ir_frame = true;
          }
        }

        // Reiniciar para esperar el próximo comando
        state = STATE_IDLE;
      }
    }
  }
}