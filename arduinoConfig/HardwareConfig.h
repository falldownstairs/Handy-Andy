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
  int minPulse = SERVOMIN;
  int maxPulse = SERVOMAX;
  int offset = 0;
};

ServoConfig servos[9] = {
  ServoConfig{0}, ServoConfig{1}, ServoConfig{2}, ServoConfig{3},
  ServoConfig{4}, ServoConfig{5}, ServoConfig{6},
  ServoConfig{7}, ServoConfig{8}
};
