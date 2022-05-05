/*
  This sketch shows the use of the getPosition method, which obtains the position of the detected line, it is very useful for line 
  following robots, there are 2 variants, the first one returns the position of the raw line and the second one receives a parameter 
  that represents the scale of values to use, for example, if a 255 is sent, the sensor bar will return the range from -255 to 255 
  where 0 represents half.

  NOTE: in case of not detecting the line, the method will return 0 if the method is called with empty arguments or a negative 
  value out of range if it is called with an integer parameter


    ╔═══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
    ║                                        600    700    800    900    1000   1100                                        ║
    ║                                 500    ███    ███    ███    ███    ███    ███    1200                                 ║
    ║                          400    ███     5      6      7      8      9      10    ███    1300                          ║
    ║                   300    ███     4                                                11    ███    1400                   ║
    ║            200    ███     3                                                              12    ███    1500            ║
    ║     100    ███     2                                                                            13    ███    1600     ║
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

  digitalWrite(LED_BUILTIN, HIGH);
  
                                 //white black white cycles through each sensor to set the detection thresholdby,
  sb.calibrate();                //default the process is done 800 times, but you can specify the value you need 
                                 //inside the parentheses
                  
                                 
  digitalWrite(LED_BUILTIN, LOW);                  

}

void loop() {
Serial.println("Positions");
  int posNormal=sb.getPosition();    //Call the method to read the position, remember it will return 0 if no line is detected
  int pos255=sb.getPosition(255);    // Will return the position in the range of -255 to 255 or a negative out of range if no line is detected
  int pos1000=sb.getPosition(1000);  // Will return the position in the range of -1000 to 1000 or a negative out of range if no line is detected
  Serial.print("Normal : ");
  Serial.print(posNormal);
  Serial.print("\t\t\t\t");
  Serial.print("Fitted (255) :");
  Serial.print(pos255);
  Serial.print("\t\t\t\t");
  Serial.print("Fitted (1000) :");
  Serial.println(pos1000);
  
  delay(1000);
}
