/*
  This sketch show how you can read the values ​​of the sensors separately  to do so you just have to call the singleSensorRead(int sensor)
  method, which will return the result of as an analog value


    (0)125       (1)130       (2)120       (3)130       (4)118      (5)988      (6)120   ....

    ╔═══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
    ║                                                       TOP VIEW                                                        ║
    ║                                        ███    ███    ███    ███    ███    ███                                         ║
    ║                                 ███     5      6      7      8      9      10    ███                                  ║
    ║                          ███     4                                                11    ███                           ║
    ║                   ███     3                                                              12    ███                    ║
    ║            ███     2                                                                            13    ███             ║
    ║     ███     1                              S2 ───────┐       ┌─────── VCC                              14    ███      ║
    ║      0                                     S3 ─────┐ │       │ ┌───── GND                                     15      ║
    ╚═══════════════════════════════════════════════════ █ █ █ █ █ █ █ ═════════════════════════════════════════════════════╝                                                       ║
                                                 S1 ─────────┘ │ └───────── OUT
                                                             S0
  Created by: Miguel Angel Delgado Lopez    /   Brandon Daniel Malagon Rodríguez
  Contact:    miguel.delgado.lop@gmail.com  /   luk.malagon@gmailcom
  Company:    Tapatio Electronics
  Date:       April 2022
*/


#include "TapatioElectronics.h"

int sensors=16;
                                  //Create object to operate the SensorBar16
SensorBar16 sb(2,3,4,5,A0);       //(S0,S1,S2,S3,OUT)
                                  //OUT must be a analog pin
                                  


void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {

  int val1 =sb.singleSensorRead(1); //Call the method to read only the sensor 1, this retunr an analog value
  Serial.println("The value of the sensor 1 is :");
  Serial.println(val1);
  int val8 =sb.singleSensorRead(8); //Call the method to read only the sensor 1, this retunr an analog value
  Serial.println("The value of the sensor 8 is :");
  Serial.println(val8);
  int val13 =sb.singleSensorRead(13); //Call the method to read only the sensor 1, this retunr an analog value
  Serial.println("The value of the sensor 13 is :");
  Serial.println(val13);
  delay(1000);
}
