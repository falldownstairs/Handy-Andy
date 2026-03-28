/*
Initial testing and calibration of servo motors connected to the
Adeept HAT, which is wired to the Arduino Mega 2560 R3 board.
*/

#include <Arduino.h>
#include "Kinematics.h"

float angle = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("Running...");

  pwm.begin();
  pwm.setOscillatorFrequency(25000000);
  pwm.setPWMFreq(50);  // Analog servos run at ~50 Hz
  delay(10);
}

void loop() {
  if (Serial.available() > 0) {
    angle = Serial.parseFloat();
    for (int i = 0; i < 2; i++) {
      writeServo(i, angle);
    }
  } 
}
