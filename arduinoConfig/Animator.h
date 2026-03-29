#pragma once
#include "Poses.h"

// State tracking
float currentPercents[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
float targetPercents[9]  = {0, 0, 0, 0, 0, 0, 0, 0, 0};
float startPercents[9]   = {0, 0, 0, 0, 0, 0, 0, 0, 0};

// Timing controls
unsigned long lastUpdateTime = 0;
int updateInterval = 15;

unsigned long poseStartTime = 0;
float poseDuration = 600.0; // How long a full pose change takes in milliseconds

// Timing and speed control for the non-easing version
float stepSize = 2.0;     // Percentage to move per tick
float jitterThreshold = 0.01; // Minimum distance to target before snapping to avoid jitter 

// Breaks a Pose struct into the flat target array and restarts the clock
void setTargetPose(Pose target) {
  for (int i = 0; i < 9; i++) {
    startPercents[i] = currentPercents[i];
  }

  for (int i = 0; i < 4; i++) { targetPercents[i] = target.fingerPercents[i]; }
  for (int i = 0; i < 3; i++) { targetPercents[i + 4] = target.splayPercents[i]; }
  for (int i = 0; i < 2; i++) { targetPercents[i + 7] = target.thumbPercents[i]; }

  poseStartTime = millis();
}

// The timing loop to be called constantly in the main loop
void updateAnimator() {
  unsigned long currentMillis = millis();

  if (currentMillis - lastUpdateTime >= updateInterval) {
    lastUpdateTime = currentMillis;

    float t = (currentMillis - poseStartTime) / poseDuration;
    if (t >= 1.0) {
      t = 1.0;
    }

    float ease = (3.0 * t * t) - (2.0 * t * t * t);

    for (int i = 0; i < 9; i++) {
      if (currentPercents[i] != targetPercents[i]) {
        currentPercents[i] = startPercents[i] + ((targetPercents[i] - startPercents[i]) * ease);
        
        if (t == 1.0) {
          currentPercents[i] = targetPercents[i];
        }

        moveServoPercent(i, currentPercents[i]);
      }
    }
  }
}

// The timing loop to be called constantly in the main loop
void updateAnimatorNoEasing() {
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
