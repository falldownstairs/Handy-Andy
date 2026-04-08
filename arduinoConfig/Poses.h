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
  const Pose rockHorns = {
    {0, 100, 100, 0},
    {100, 100, 100},
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
  const Pose D = {
    {0, 0, 0, 100},
    {0, 0, 0},
    {50, 100}
  }; 
  const Pose E = {
    {100, 100, 100, 100},
    {0, 0, 0},
    {50, 100}
  };
  const Pose F = {
    {0, 100, 100, 100},
    {0, 0, 0},
    {50, 100}
  }; 
  const Pose G = {
    {50, 50, 50, 50},
    {0, 0, 0},
    {50, 100}
  };
  const Pose H = {
    {0, 0, 0, 0},
    {0, 0, 0},
    {50, 100}
  };
  const Pose I = {
    {100, 100, 100, 100},
    {0, 0, 0},
    {50, 100}
  };
  const Pose J = {
    {100, 100, 100, 100},
    {0, 0, 0},
    {50, 100}
  };
  const Pose K = {
    {0, 100, 100, 100},
    {0, 0, 0},
    {50, 100}
  };
  const Pose L = {
    {0, 0, 0, 100},
    {0, 0, 0},
    {50, 100}
  };
  const Pose M = {
    {100, 100, 100, 100},
    {0, 0, 0},
    {50, 100}
  };
  const Pose N = {
    {100, 100, 100, 100},
    {0, 0, 0},
    {50, 100}
  };
  const Pose O = {  
    {50, 50, 50, 50},
    {0, 0, 0},
    {50, 100}
  };
  const Pose P = {
    {0, 100, 100, 100},
    {0, 0, 0},
    {50, 100}
  };
  const Pose Q = {
    {50, 50, 50, 50},
    {0, 0, 0},
    {50, 100}
  };
  const Pose R = {
    {0, 100, 100, 100},
    {0, 0, 0},
    {50, 100}
  };
  const Pose S = {
    {100, 100, 100, 100},
    {0, 0, 0},
    {50, 100}
  };
  const Pose T = {
    {100, 100, 100, 100},
    {0, 0, 0},
    {50, 100}
  };
  const Pose U = {
    {0, 0, 0, 0},
    {0, 0, 0},
    {50, 100}
  };
  const Pose V = {
    {0, 0, 0, 0},
    {0, 0, 0},
    {50, 100}
  };
  const Pose W = {
    {0, 0, 0, 0},
    {0, 0, 0},
    {50, 100}
  };
  const Pose X = {
    {100, 100, 100, 100},
    {0, 0, 0},  
    {50, 100}
  };
  const Pose Y = {
    {100, 100, 100, 100},
    {0, 0, 0},
    {50, 100}
  };
  const Pose Z = {
    {100, 100, 100, 100},
    {0, 0, 0},
    {50, 100}
  };
}
