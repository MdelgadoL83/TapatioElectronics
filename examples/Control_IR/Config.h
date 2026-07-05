/*
 * ==========================================
 *    ARCHIVO DE CONFIGURACIÓN - CONTROL DE JUEZ MARCA TAPATIO ELECTRONICS
 * ==========================================
 * INSTRUCCIONES:
 * - Solo modifica los valores dentro de la sección "PARÁMETROS CONFIGURABLES".
 * - NO MODIFIQUES los valores de "TIMINGS DEL PROTOCOLO" a menos que sepas 
 *   exactamente lo que estás haciendo.
 * - IMPORTANTE: Los comandos deben ser valores entre 0x00 y 0x7F (máximo 127).
 * Created by: Miguel Angel Delgado Lopez
 * Contact:    miguel.delgado.lop@gmail.com
 * Company:    Tapatio Electronics
 * Date:       April 2022
 */

#ifndef CONFIG_H
#define CONFIG_H

// ==========================================
// PARÁMETROS CONFIGURABLES (PUEDES CAMBIAR)
// ==========================================
#define CMD_RESET 0x15  
#define CMD_RDY   0x34  
#define CMD_GO    0x11  

// ==========================================
// COMPORTAMIENTO DEL TRANSMISOR (PUEDES CAMBIAR)
// ==========================================
#define TX_IDLE_TIMEOUT_MS  5000  
#define TX_IDLE_FLASH_MS    20  

// ==========================================
// DIRECCIÓN IR (Identifica tu control, evita cruce con otros controles)
// ==========================================
#define IR_ADDRESS 0x01  

// ==========================================
//  PINES DEL TRANSMISOR (NO TOCAR)
// ==========================================
#define TX_IR_SEND_PIN PIN_PA2
#define TX_BTN_RESET   PIN_PA6
#define TX_BTN_RDY     PIN_PA1
#define TX_BTN_GO      PIN_PA3
#define TX_LEDS        PIN_PA7


// ==========================================
// TIMINGS DEL PROTOCOLO SONY SIRC (NO TOCAR)
// ==========================================
#define SONY_HDR_MARK  2400
#define SONY_BIT_ONE   1200
#define SONY_BIT_ZERO  600
#define TOLERANCIA     250

#endif