/*
This sketch show how you can read analog values from SensorBar16 calling the analorRead method that returns an array of size 
16 with each sensor value

Each sensor, when read raw, will return a value between 0 and 1024 (arduino 10bit resolution) depending on the color level found 
tending to 1024 for black and tending to 0 for white

In Serial Monitor you can see the 16 sensors numered from 0 to 15 insead parentesis, and the real value at the right such as:

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
                                            S1 ─────────┘  │ └───────── OUT
                                                          S0

Created by: Miguel Angel Delgado Lopez    /   Brandon Daniel Malagon Rodríguez
Contact:    miguel.delgado.lop@gmail.com  /   luk.malagon@gmailcom
Company:    Tapatio Electronics
Date:       April 2022
*/


#include "TapatioElectronics.h"

int sensors=16;
SensorBar16 sb(2,3,4,5,A0);    //Create object to operate the SensorBar16


void setup() {
Serial.begin(115200);
Serial.println();
Serial.println("-----------------------------------------------------------------------------");
Serial.println("    Basic Example for Reading RAW values form SensorBar16");
Serial.println("      adjust the IDE font size to correctly display the data");
Serial.println("-----------------------------------------------------------------------------");

}

void loop() {

int* values=sb.analogValues();     //Call the method to read values 
Serial.print("");
for(int x=0;x<sensors;x++){
Serial.print("(");
Serial.print(x);
Serial.print(") ");
Serial.print(values[x]);    //Access each of the values
Serial.print("\t");
}
Serial.println();
delay(1000);
}
