#pragma once
#include <Arduino.h>
#include "Animator.h"
#include "Commands.h"
#include "StateReporter.h"

const float RAW_MOVE_DURATION_MS = 140.0;

float clampPercentForRaw(float value) {
  if (value < 0.0) {
    return 0.0;
  }
  if (value > 100.0) {
    return 100.0;
  }
  return value;
}

bool parseRawServoCommand(const String& rawCommand, float outValues[SERVO_COUNT]) {
  if (!rawCommand.startsWith("raw,")) {
    return false;
  }

  int start = 4;
  for (int i = 0; i < SERVO_COUNT; i++) {
    int commaIndex = rawCommand.indexOf(',', start);
    String token = (commaIndex == -1) ? rawCommand.substring(start) : rawCommand.substring(start, commaIndex);
    token.trim();

    if (token.length() == 0) {
      return false;
    }

    bool isValidNumber = true;
    bool hasDigit = false;
    bool hasDot = false;
    for (unsigned int c = 0; c < token.length(); c++) {
      char ch = token.charAt(c);
      if (ch >= '0' && ch <= '9') {
        hasDigit = true;
        continue;
      }
      if (ch == '.' && !hasDot) {
        hasDot = true;
        continue;
      }
      if ((ch == '-' || ch == '+') && c == 0) {
        continue;
      }
      isValidNumber = false;
      break;
    }

    if (!isValidNumber || !hasDigit) {
      return false;
    }

    outValues[i] = clampPercentForRaw(token.toFloat());

    if (i < SERVO_COUNT - 1) {
      if (commaIndex == -1) {
        return false;
      }
      start = commaIndex + 1;
    } else if (commaIndex != -1) {
      return false;
    }
  }

  return true;
}

void applyRawServoValues(const float values[SERVO_COUNT]) {
  setTargetPercents(values, RAW_MOVE_DURATION_MS);
}

void sendCommandsList() {
  Serial.print("COMMANDS_LIST:[");
  for (int i = 0; i < NUM_COMMANDS; i++) {
    Serial.print("\"");
    Serial.print(commands[i].trigger);
    Serial.print("\"");
    if (i < NUM_COMMANDS - 1) {
      Serial.print(",");
    }
  }
  Serial.println("]");
}

void handleIncomingCommand(const String& command, unsigned long& signCompleteTime, bool& waitingForDone) {
  if (command.equalsIgnoreCase("GET_COMMANDS")) {
    sendCommandsList();
    return;
  }

  if (command.startsWith("raw,")) {
    float values[SERVO_COUNT];
    if (parseRawServoCommand(command, values)) {
      applyRawServoValues(values);
      Serial.println("RAW OK");
      sendStateUpdate(true);
    } else {
      Serial.println("RAW ERR");
    }
    return;
  }

  Pose pose;
  const char* trigger = nullptr;
  if (findPoseForCommand(command, pose, trigger)) {
    setTargetPose(pose);
    Serial.print("Signing: ");
    Serial.println(trigger);
    signCompleteTime = millis() + (unsigned long)poseDuration + 500;
    waitingForDone = true;
    return;
  }

  Serial.print("Unknown command: ");
  Serial.println(command);
}
