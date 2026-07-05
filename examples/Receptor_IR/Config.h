/*
 * ==========================================
 *    ARCHIVO DE CONFIGURACIÓN - CONTROL DE JUEZ
 * ==========================================
 * INSTRUCCIONES:
 * - Solo modifica los valores dentro de la sección "PARÁMETROS CONFIGURABLES".
 * - NO MODIFIQUES los valores de "TIMINGS DEL PROTOCOLO" a menos que sepas 
 *   exactamente lo que estás haciendo.
 * - IMPORTANTE: Los comandos deben ser valores entre 0x00 y 0x7F (máximo 127) y correponder con los del emisor.
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
// COMPORTAMIENTO DEL RECEPTOR
#define RX_IGNORE_ADDRESS true
// ==========================================

// ==========================================
// DIRECCIÓN IR (Identifica tu control, evita cruce con otros controles)
// ==========================================
#define IR_ADDRESS 0x01  // Dirección del control (usada si RX_IGNORE_ADDRESS es false)

// ==========================================
//  PINES DEL RECEPTOR (NO TOCAR)
// ==========================================
#define RX_IR_PIN      PIN_PA1
#define RX_PIN_RDY     PIN_PA6
#define RX_PIN_GO      PIN_PA7


// ==========================================
// TIMINGS DEL PROTOCOLO SONY SIRC (NO TOCAR)
// ==========================================
#define SONY_HDR_MARK  2400
#define SONY_BIT_ONE   1200
#define SONY_BIT_ZERO  600
#define TOLERANCIA     250

#endif