#pragma once
#include "HardwareConfig.h"

// Writes an angle to the servo, 0 <= degree <= 160
void writeServo(int servoIndex, float degree) {
  ServoConfig s = servos[servoIndex];
  if (degree < 0.0) {
    degree = 0.0;
  } else if (degree > 160.0) {
    degree = 160.0;
  }
  float exactPulse = (degree - 0.0) * (s.maxPulse - s.minPulse) / 160.0 + s.minPulse;
  int pulse = round(exactPulse);
  pwm.setPWM(s.port, 0, pulse);
}

// Moves a servo based on percentage
void moveServoPercent(int servoIndex, float percent) {
  ServoConfig s = servos[servoIndex];
  if (percent < 0.0) {
    percent = 0.0;
  } else if (percent > 100.0) {
    percent = 100.0;
  }
  float exactPulse = s.minPulse + percent * (s.maxPulse - s.minPulse) / 100.0;
  int pulse = round(exactPulse);
  pwm.setPWM(s.port, 0, pulse);
}

// Moves a finger (0..3)
void setFinger(int fingerIndex, float percent) {
  moveServoPercent(fingerServos[fingerIndex], percent);
}

// Moves all fingers to the same percentage
void setFingers(float percent) {
  for (int s: fingerServos) {
    setFinger(s, percent);
  }
}

// Moves fingers to different percentages, -1 means don't move that finger
void setFingers(float percents[]) {
  for (int i = 0; i < 4; i++) {
    if (percents[i] != -1) {
      setFinger(i, percents[i]);
    }
  }
}

// Splays fingers apart, leftFinger and rightFinger must be adjacent
void setSplay(int leftFinger, int rightFinger, float percent) {
  if (leftFinger == pinkieFinger || rightFinger == indexFinger || rightFinger - leftFinger != 1) {
    return;
  }

  if (leftFinger == indexFinger) {
    moveServoPercent(splayServos[0], percent);
  } else if (leftFinger == middleFinger) {
    moveServoPercent(splayServos[1], percent);
  } else if (leftFinger == ringFinger) {
    moveServoPercent(splayServos[2], percent);
  }
}

// Splays all fingers apart to the same percentage
void setSplay(float percent) {
  for (int s: splayServos) {
    moveServoPercent(s, percent);
  }
}

// Splays fingers apart to different percentages, -1 means don't move that splay
void setSplay(float percents[]) {
  for (int i = 0; i < 3; i++) {
    if (percents[i] != -1) {
      moveServoPercent(splayServos[i], percents[i]);
    }
  }
}
