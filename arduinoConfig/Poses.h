#pragma once
#include "Kinematics.h"

struct Pose {
  float fingerPercents[4];
  float splayPercents[3];
  float thumbPercents[2];
};

namespace Gestures {
  const Pose openHand = {
    {0, 0, 0, 0},
    {0, 0, 0},
    {0, 0}
  };

  const Pose openHandSplayed = {
    {0, 0, 0, 0},
    {100, 100, 100},
    {0, 0}
  };
  
  const Pose closedFist = {
    {100, 100, 100, 100},
    {0, 0, 0},
    {0, 100}
  };
  
  const Pose pointIndex = {
    {0, 100, 100, 100},
    {0, 0, 0},
    {50, 100}
  };
  
  const Pose thumbsUp = {
    {100, 100, 100, 100},
    {0, 0, 0},
    {0, 0}
  };
}

namespace Letters {
  const Pose A = {
    {100, 100, 100, 100},
    {0, 0, 0},
    {0, 50}
  };

  const Pose B = {
    {0, 0, 0, 0},
    {0, 0, 0},
    {20, 100}
  };

  const Pose C = {
    {50, 50, 50, 50},
    {20, 20, 20},
    {50, 50}
  };
}
