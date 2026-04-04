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

void setup() {
  Serial.begin(9600);
  Serial.println("Running...");

  pwm.begin();
  pwm.setOscillatorFrequency(25000000);
  pwm.setPWMFreq(50);  // Analog servos run at ~50 Hz
  delay(10);

  setTargetPose(Gestures::openHand);
}

void loop() {
  updateAnimator();

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
