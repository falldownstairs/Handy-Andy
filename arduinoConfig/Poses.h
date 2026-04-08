#pragma once
#include "Kinematics.h"

struct Pose {
  float fingerPercents[4]; // Index, Middle, Ring, Pinkie (0 = extended, 100 = folded)
  float splayPercents[3];  // Index-Middle, Middle-Ring, Ring-Pinkie (0 = closed, 100 = splayed)
  float thumbPercents[2];  // Bend (0 = extended, 100 = folded), Cross (0 = beside, 100 = across palm)
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
    {0, 20} // Thumb rests against the side of the folded index finger
  };

  const Pose B = {
    {0, 0, 0, 0},
    {0, 0, 0},
    {50, 100} // Thumb folded across the palm
  };

  const Pose C = {
    {50, 50, 50, 50},
    {0, 0, 0},
    {50, 20} // Fingers and thumb uniformly curved
  };

  const Pose D = {
    {0, 80, 80, 80},
    {0, 0, 0},
    {50, 50} // Index up, others curved to meet the thumb
  }; 
  
  const Pose E = {
    {100, 100, 100, 100},
    {0, 0, 0},
    {80, 80} // Fingers folded tightly, thumb folded closely underneath
  };

  const Pose F = {
    {80, 0, 0, 0},
    {0, 50, 50}, // Index meets thumb, others extended and slightly splayed
    {50, 50}
  }; 
  
  const Pose G = {
    {0, 100, 100, 100},
    {0, 0, 0},
    {0, 0} // Index and thumb parallel (sideways orientation handled by arm, not hand)
  };

  const Pose H = {
    {0, 0, 100, 100},
    {0, 0, 0}, // Index and middle together, others folded
    {20, 50}
  };

  const Pose I = {
    {100, 100, 100, 0},
    {0, 0, 0},
    {50, 100} // Pinkie up, thumb over folded fingers
  };

  const Pose J = {
    {100, 100, 100, 0}, // Same static pose as 'I' (requires arm motion to draw the 'J')
    {0, 0, 0},
    {50, 100}
  };

  const Pose K = {
    {0, 0, 100, 100},
    {100, 0, 0}, // Index and middle extended & splayed, thumb rests on middle
    {20, 40}
  };

  const Pose L = {
    {0, 100, 100, 100},
    {0, 0, 0},
    {0, 0} // Index up, thumb fully extended to the side
  };

  const Pose M = {
    {100, 100, 100, 100},
    {0, 0, 0},
    {0, 100} // Thumb tucked under index, middle, and ring fingers
  };

  const Pose N = {
    {100, 100, 100, 100},
    {0, 0, 0},
    {0, 80} // Thumb tucked under index and middle fingers
  };

  const Pose O = {  
    {50, 50, 50, 50},
    {0, 0, 0},
    {50, 80} // Fingers curved, thumb touches the tips
  };

  const Pose P = {
    {0, 0, 100, 100}, // Same static pose as 'K', but arm is pointed down
    {100, 0, 0},
    {20, 40}
  };

  const Pose Q = {
    {0, 100, 100, 100}, // Same static pose as 'G', but arm is pointed down
    {0, 0, 0},
    {0, 0}
  };

  const Pose R = {
    {0, 0, 100, 100},
    {0, 0, 0}, // Fingers crossed in human ASL. Hard for robots, so setting them straight and tight.
    {50, 100}
  };

  const Pose S = {
    {100, 100, 100, 100},
    {0, 0, 0},
    {50, 100} // Fist, but thumb crosses OVER the folded fingers
  };

  const Pose T = {
    {100, 100, 100, 100},
    {0, 0, 0},
    {0, 50} // Thumb tucked ONLY under the index finger
  };

  const Pose U = {
    {0, 0, 100, 100},
    {0, 0, 0}, // Index and middle extended together, closed splay
    {50, 100}
  };

  const Pose V = {
    {0, 0, 100, 100},
    {100, 0, 0}, // Index and middle extended and splayed
    {50, 100}
  };

  const Pose W = {
    {0, 0, 0, 100},
    {100, 100, 0}, // Index, middle, ring extended and splayed
    {50, 100} // Thumb holds down the pinkie
  };

  const Pose X = {
    {50, 100, 100, 100}, // Index finger curled into a hook
    {0, 0, 0},  
    {50, 100}
  };

  const Pose Y = {
    {100, 100, 100, 0},
    {0, 0, 0},
    {0, 0} // Pinkie up, thumb fully extended side
  };

  const Pose Z = {
    {0, 100, 100, 100}, // Index extended to trace the 'Z', others folded
    {0, 0, 0},
    {50, 100}
  };
}