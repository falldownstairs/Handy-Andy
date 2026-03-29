#pragma once
#include "Poses.h"

// State tracking for all 9 servos
float currentPercents[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
float targetPercents[9]  = {0, 0, 0, 0, 0, 0, 0, 0, 0};

// Timing and speed control
unsigned long lastUpdateTime = 0;
int updateInterval = 15;  // Milliseconds between micro-movements
float stepSize = 2.0;     // Percentage to move per tick
float jitterThreshold = 0.01; // Minimum distance to target before snapping to avoid jitter

// Breaks a Pose struct into the flat target array
void setTargetPose(Pose target) {
  for (int i = 0; i < 4; i++) {
    targetPercents[i] = target.fingerPercents[i];
  }
  for (int i = 0; i < 3; i++) {
    targetPercents[i + 4] = target.splayPercents[i];
  }
  for (int i = 0; i < 2; i++) {
    targetPercents[i + 7] = target.thumbPercents[i];
  }
}

// The timing loop to be called constantly in the main loop
void updateAnimator() {
  unsigned long currentMillis = millis();

  if (currentMillis - lastUpdateTime >= updateInterval) {
    lastUpdateTime = currentMillis;

    for (int i = 0; i < 9; i++) {
      float distance = targetPercents[i] - currentPercents[i];
      
      if (fabs(distance) > jitterThreshold) {
        if (fabs(distance) <= stepSize) {
          currentPercents[i] = targetPercents[i];
        } else if (distance > 0) {
          currentPercents[i] += stepSize;
        } else {
          currentPercents[i] -= stepSize;
        }

        moveServoPercent(i, currentPercents[i]);
      }
    }
  }
}
