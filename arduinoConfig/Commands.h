#pragma once
#include <Arduino.h>
#include "Poses.h"

struct Command {
  const char* trigger;
  Pose pose;
};

const Command commands[] = {
  {"open", Gestures::openHand},
  {"spread", Gestures::openHandSplayed},
  {"fist", Gestures::closedFist},
  {"point", Gestures::pointIndex},
  {"thumbs up", Gestures::thumbsUp},
  {"rock horns", Gestures::rockHorns}
};

const Command alphabet[] = {
  {"A", Letters::A},
  {"B", Letters::B},
  {"C", Letters::C},
  {"D", Letters::D},
  {"E", Letters::E},
  {"F", Letters::F},
  {"G", Letters::G},
  {"H", Letters::H},
  {"I", Letters::I},
  {"J", Letters::J},
  {"K", Letters::K},
  {"L", Letters::L},
  {"M", Letters::M},
  {"N", Letters::N},
  {"O", Letters::O},
  {"P", Letters::P},
  {"Q", Letters::Q},
  {"R", Letters::R},
  {"S", Letters::S},
  {"T", Letters::T},
  {"U", Letters::U},
  {"V", Letters::V},
  {"W", Letters::W},
  {"X", Letters::X},
  {"Y", Letters::Y},
  {"Z", Letters::Z}
};

const int NUM_COMMANDS = sizeof(commands) / sizeof(commands[0]);
const int NUM_ALPHABET = sizeof(alphabet) / sizeof(alphabet[0]);

bool findPoseForCommand(const String& input, Pose& outPose, const char*& outTrigger) {
  for (int i = 0; i < NUM_COMMANDS; i++) {
    if (input.equalsIgnoreCase(commands[i].trigger)) {
      outPose = commands[i].pose;
      outTrigger = commands[i].trigger;
      return true;
    }
  }

  for (int i = 0; i < NUM_ALPHABET; i++) {
    if (input.equalsIgnoreCase(alphabet[i].trigger)) {
      outPose = alphabet[i].pose;
      outTrigger = alphabet[i].trigger;
      return true;
    }
  }

  return false;
}
