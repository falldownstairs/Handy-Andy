#pragma once
#include <Arduino.h>
#include "Animator.h"

const unsigned long STATE_UPDATE_INTERVAL_MS = 120;

int lastReportedTenths[SERVO_COUNT] = {-1, -1, -1, -1, -1, -1, -1, -1, -1};
unsigned long lastStateUpdateTime = 0;

float clampPercentForState(float value) {
  if (value < 0.0) {
    return 0.0;
  }
  if (value > 100.0) {
    return 100.0;
  }
  return value;
}

int roundedServoTenths(float value) {
  int v = (int)round(clampPercentForState(value) * 10.0);
  if (v < 0) {
    return 0;
  }
  if (v > 1000) {
    return 1000;
  }
  return v;
}

void sendStateUpdate(bool force) {
  unsigned long now = millis();
  if (!force && (now - lastStateUpdateTime) < STATE_UPDATE_INTERVAL_MS) {
    return;
  }

  bool changed = force;
  int roundedTenths[SERVO_COUNT];
  for (int i = 0; i < SERVO_COUNT; i++) {
    roundedTenths[i] = roundedServoTenths(currentPercents[i]);
    if (!changed && roundedTenths[i] != lastReportedTenths[i]) {
      changed = true;
    }
  }

  if (!changed) {
    return;
  }

  Serial.print("STATE");
  for (int i = 0; i < SERVO_COUNT; i++) {
    Serial.print(",");
    Serial.print(roundedTenths[i] / 10.0, 1);
    lastReportedTenths[i] = roundedTenths[i];
  }
  Serial.println();

  lastStateUpdateTime = now;
}
