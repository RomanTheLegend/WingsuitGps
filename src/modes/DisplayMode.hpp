#pragma once

#include <Arduino.h>
#include "../modules/ButtonInterface.hpp"

class DisplayMode
{
public:
  virtual void init() = 0;
  virtual void processInput(ButtonEvent event) = 0;
  virtual void display() = 0;
};