/*
Initial testing and calibration of servo motors connected to the
Adeept HAT, which is wired to the Arduino Mega 2560 R3 board.

Connections from HAT P1 Header to Arduino:
Pin 1 (3.3V) -> 3.3V
Pin 3 (SDA1) -> 20 (SDA)
Pin 5 (SCL1) -> 21 (SCL)
Pin 6 (GND)  -> GND

Referenced from Adeept_RaspClaws-V2.0-20251030/Circuit-Schematic-Adeept_Robot-HAT.pdf
*/ 

#include <Arduino.h>
#include "Animator.h"
#include "StateReporter.h"
#include "SerialProtocol.h"

bool waitingForDone = false;
unsigned long signCompleteTime = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("Running...");
  sendCommandsList();

  pwm.begin();
  pwm.setOscillatorFrequency(25000000);
  pwm.setPWMFreq(50);  // Analog servos run at ~50 Hz
  delay(10);

  setTargetPose(Gestures::openHand);
  sendStateUpdate(true);
}

void loop() {
  updateAnimator();
  sendStateUpdate(false);

  if (waitingForDone) {
    if (millis() >= signCompleteTime) {
      Serial.println("DONE");
      waitingForDone = false;
    }
    return;
  }

  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    handleIncomingCommand(command, signCompleteTime, waitingForDone);
  }
}
