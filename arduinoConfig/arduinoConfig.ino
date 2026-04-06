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

String command = "";

struct Command {
  String trigger;
  Pose pose;
};

Command commands[] = {
  Command{"open", Gestures::openHand},
  Command{"spread", Gestures::openHandSplayed},
  Command{"fist", Gestures::closedFist},
  Command{"point", Gestures::pointIndex},
  Command{"thumbs up", Gestures::thumbsUp},
};

Command Alphabet[] = {
  Command{"A", Letters::A},
  Command{"B", Letters::B},
  Command{"C", Letters::C},
  Command{"D", Letters::D},
  Command{"E", Letters::E},
  Command{"F", Letters::F},
  Command{"G", Letters::G},
  Command{"H", Letters::H},
  Command{"I", Letters::I},
  Command{"J", Letters::J},
  Command{"K", Letters::K},
  Command{"L", Letters::L},
  Command{"M", Letters::M},
  Command{"N", Letters::N},
  Command{"O", Letters::O},
  Command{"P", Letters::P},
  Command{"Q", Letters::Q},
  Command{"R", Letters::R},
  Command{"S", Letters::S},
  Command{"T", Letters::T},
  Command{"U", Letters::U},
  Command{"V", Letters::V},
  Command{"W", Letters::W},
  Command{"X", Letters::X},
  Command{"Y", Letters::Y},
  Command{"Z", Letters::Z}
};

const int NUM_COMMANDS = sizeof(commands) / sizeof(commands[0]);
const int NUM_ALPHABET = sizeof(Alphabet) / sizeof(Alphabet[0]);

bool waitingForDone = false;
unsigned long signCompleteTime = 0;

float angle = 0;

const float RAW_MOVE_DURATION_MS = 140.0;
const unsigned long STATE_UPDATE_INTERVAL_MS = 120;

int lastReportedPercents[SERVO_COUNT] = {-1, -1, -1, -1, -1, -1, -1, -1, -1};
unsigned long lastStateUpdateTime = 0;

float clampPercent(float value) {
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

    outValues[i] = clampPercent(token.toFloat());

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

int roundedServoPercent(float value) {
  int v = (int)round(clampPercent(value));
  if (v < 0) {
    return 0;
  }
  if (v > 100) {
    return 100;
  }
  return v;
}

void applyRawServoValues(const float values[SERVO_COUNT]) {
  setTargetPercents(values, RAW_MOVE_DURATION_MS);
}

void sendStateUpdate(bool force) {
  unsigned long now = millis();
  if (!force && (now - lastStateUpdateTime) < STATE_UPDATE_INTERVAL_MS) {
    return;
  }

  bool changed = force;
  int roundedValues[SERVO_COUNT];
  for (int i = 0; i < SERVO_COUNT; i++) {
    roundedValues[i] = roundedServoPercent(currentPercents[i]);
    if (!changed && roundedValues[i] != lastReportedPercents[i]) {
      changed = true;
    }
  }

  if (!changed) {
    return;
  }

  String stateMessage = "STATE";
  for (int i = 0; i < SERVO_COUNT; i++) {
    stateMessage += ",";
    stateMessage += roundedValues[i];
    lastReportedPercents[i] = roundedValues[i];
  }

  Serial.println(stateMessage);
  lastStateUpdateTime = now;
}

void setup() {
  Serial.begin(9600);
  Serial.println("Running...");

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
    command = Serial.readStringUntil('\n');
    command.trim();

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

    bool commandFound = false;

    for (int i = 0; i < NUM_COMMANDS; i++) {
      if (command.equalsIgnoreCase(commands[i].trigger)) {
        setTargetPose(commands[i].pose);
        Serial.println("Signing: " + commands[i].trigger);
        commandFound = true;
        break;
      }
    }

    if (!commandFound) {
      for (int i = 0; i < NUM_ALPHABET; i++) {
        if (command.equalsIgnoreCase(Alphabet[i].trigger)) {
          setTargetPose(Alphabet[i].pose);
          Serial.println("Signing: " + Alphabet[i].trigger);
          commandFound = true;
          break;
        }
      }
    }

    if (commandFound) {
      signCompleteTime = millis() + (unsigned long)poseDuration + 500;
      waitingForDone = true;
    } else {
      Serial.println("Unknown command: " + command);
    }
  }
}
