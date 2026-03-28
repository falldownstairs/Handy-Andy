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

// Each finger has a different length
float fingerProportionality[4] = {1.0, 1.0, 1.0, 1.0};

struct ServoConfig {
  int port;
  int minPulse;
  int maxPulse;
  int offset;
};

ServoConfig servos[9] = {
  ServoConfig{0, SERVOMIN, SERVOMAX, 0}, ServoConfig{1, SERVOMIN, SERVOMAX, 0}, ServoConfig{2, SERVOMIN, SERVOMAX, 0}, ServoConfig{3, SERVOMIN, SERVOMAX, 0},
  ServoConfig{4, SERVOMIN, SERVOMAX, 0}, ServoConfig{5, SERVOMIN, SERVOMAX, 0}, ServoConfig{6, SERVOMIN, SERVOMAX, 0},
  ServoConfig{7, SERVOMIN, SERVOMAX, 0}, ServoConfig{8, SERVOMIN, SERVOMAX, 0}
};
