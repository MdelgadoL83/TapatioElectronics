/*
  Test Potentiometers - Tapatio Follower Kit

  This sketch tests the potentiometers, OLED display, and button of the
  Tapatio Follower Kit. It displays real-time readings on the OLED screen.

  Components tested:
  - 4 Potentiometers (connected to A7, A6, A1, A0)
  - OLED Display SSD1306 (128x32 pixels, I2C address 0x3C)
  - Button (connected to pin 2)

  The display shows:
  - Analog values from all 4 potentiometers
  - Digital state of the button (1 = not pressed, 0 = pressed)

  Created by: Miguel Angel Delgado Lopez
  Contact:    miguel.delgado.lop@gmail.com
  Company:    Tapatio Electronics
  Date:       Jan 2026
*/
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <TapatioElectronics.h>
#include <Wire.h>

//-----------------------------------SCREEN
// VARS---------------------------------------------------
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define SCREEN_ADDRESS                                                         \
  0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);
String cad;

//-----------------------------------POTS
// & BUTTON VARS------------------------------------------
int pots[] = {A7, A6, A1, A0};
#define boton 2

void setup() {
  //-----------------------------------SCREEN------------------------------------------
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  SplashScreen(display);
  delay(3000);
  display.clearDisplay();
  display.setTextSize(1);              // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text

  //-----------------------------------POTENCIOMETROS------------------------------------------
  for (int x = 0; x < 4; x++) {
    pinMode(pots[x], INPUT);
  }

  //-----------------------------------BUTTON------------------------------------------
  pinMode(boton, INPUT_PULLUP);
}

void loop() {
  //-----------------------------------SCREEN------------------------------------------
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Pots: ");
  for (int x = 0; x < 4; x++) {
    display.print(analogRead(pots[x]));
    display.print(" ");
  }

  display.print("Boton: ");
  display.print(digitalRead(boton));
  display.display();
  delay(100);
}
