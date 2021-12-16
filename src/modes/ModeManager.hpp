#pragma once

// Warning: This is a code example for the article "guideline for modular firmware".
//          It is code in a transition phase, do not use it as it is!

#include <Arduino.h>
#include "../modules/ButtonInterface.hpp"

enum Mode : uint8_t {
  Menu = 0,
  HorisontalSpeed = 1,
  VerticalSpeed = 2,
  TotalSpeed = 3,
  Heading = 4
};

namespace ModeManager {

  void init();

  void displayMode(Mode mode);

  Mode getMode();

  void progress();

  void processInput(ButtonEvent event);

}