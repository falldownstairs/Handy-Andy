#pragma once
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// Default I2C address for the PCA9685 is 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// Standard servo pulse widths
#define SERVOMIN  200
#define SERVOMAX  580

int fingerServos[4] = {0, 1, 2, 3};
int splayServos[3] = {4, 5, 6};
int thumbServos[2] = {7, 8};

int indexFinger = fingerServos[0];
int middleFinger = fingerServos[1];
int ringFinger = fingerServos[2];
int pinkieFinger = fingerServos[3];

struct Servo {
  int port;
  int minPulse;
  int maxPulse;
  int offset;
  float multiplier;
};

Servo servos[9] = {
  // index finger
  Servo{0, SERVOMIN, SERVOMAX, 0, 1.0},
  // middle finger
  Servo{1, SERVOMIN, SERVOMAX, 0, 1.0},
  // ring finger
  Servo{2, SERVOMIN, SERVOMAX, 0, 1.0},
  // pinkie finger
  Servo{3, SERVOMIN, SERVOMAX, 0, 1.0},
  // index-middle splay
  Servo{4, SERVOMIN, SERVOMAX, 0, 1.0},
  // middle-ring splay
  Servo{5, SERVOMIN, SERVOMAX, 0, 1.0},
  // ring-pinkie splay
  Servo{6, SERVOMIN, SERVOMAX, 0, 1.0},
  // bend thumb
  Servo{7, SERVOMIN, SERVOMAX, 0, 1.0},
  // cross thumb
  Servo{8, SERVOMIN, SERVOMAX, 0, 1.0}
};
