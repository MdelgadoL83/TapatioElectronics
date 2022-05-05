/*
  This sketch show how you can read analog values from SensorBar16 calling the digitalRead method that returns an array of size 
  16 with each sensor value

  Each sensor, when read as digital, will return a value designed for: (sensorNumber+1)*100, in other words 100 for the first sensor 
  and 1600 for the last one, failing that 0 if the sensor does not detect a black line; it is extremely important to calibrate the 
  SensorBar16 going from white black white each sensor, when the calibrate() method is called there is a time to do this process


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

  int* values=sb.digitalValues(); //Call the method to read values 
  for(int x=0;x<sensors;x++){
    Serial.print("(");
    Serial.print(x);
    Serial.print(") ");
    Serial.print(values[x]);     //Access each of the values
    Serial.print("\t");
  }
  Serial.println();
  delay(1000);
}
