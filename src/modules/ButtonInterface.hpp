#pragma once

#include <Arduino.h>

  enum ButtonEvent {
    NONE,
    UP, 
    DOWN, 
    ENTER,
    EXIT
  };

namespace ButtonInterface { 



  typedef void (*Function)();

  void init();

  ButtonEvent getLastEvent();

  void setCallback(Function fn);

}