/*
  Test Motors - Line Follower Kit

  This sketch tests that the motors mounted on the Line Follower Kit board
  rotate in the correct direction. Motors should rotate forward (clockwise)
  when positive velocity is set or backward (counterclockwise) when negative
  velocity is set.

  Pin connections for TB6612FNG Driver:
  - Pin 3 (pwma): PWM Motor A - Left motor speed control
  - Pin 4 (ain2): AIN2 - Motor A direction (low bit)
  - Pin 5 (ain1): AIN1 - Motor A direction (high bit)
  - Pin 7 (bin1): BIN1 - Motor B direction (high bit)
  - Pin 6 (bin2): BIN2 - Motor B direction (low bit)
  - Pin 9 (pwmb): PWM Motor B - Right motor speed control

  Note: PWM pins (3 and 9) are configured to 976Hz to reduce audible noise.

  Created by: Miguel Angel Delgado Lopez
  Contact:    miguel.delgado.lop@gmail.com
  Company:    Tapatio Electronics
  Date:       Jan 2026
*/

#include <TapatioElectronics.h>

// Pin definitions for TB6612FNG
int pwma = 3; // PWM Motor A (Timer2)
int ain2 = 4; // AIN2 - Motor A Direction
int ain1 = 5; // AIN1 - Motor A Direction
int bin1 = 7; // BIN1 - Motor B Direction
int bin2 = 6; // BIN2 - Motor B Direction
int pwmb = 9; // PWM Motor B (Timer1)

// Create driver instance
DriverTB6612 motors(pwma, ain1, ain2, pwmb, bin1, bin2);

// Test parameters
#define SPEED 60
#define FORWARD true // true = forward, false = backward

void setup() {
  // Configure timers for PWM at ~976Hz (prescaler 64)
  // This reduces audible motor noise (below 1kHz is audible)
  TCCR1B = TCCR1B & B11111000 |
           B00000011; // Timer1: D9 and D10, 16MHz/(64*256)=976Hz
  TCCR2B = TCCR2B & B11111000 |
           B00000100; // Timer2: D3 and D11, 16MHz/(64*256)=976Hz
}

void loop() {
  // Test motor direction
#if FORWARD
  motors.setVelocidad(SPEED, SPEED); // Both motors forward
#else
  motors.setVelocidad(-SPEED, -SPEED); // Both motors backward
#endif
  delay(2000); // Keep spinning for 2 seconds

  // Pause to observe behavior
  motors.setVelocidad(0, 0);
  delay(1000);
}
