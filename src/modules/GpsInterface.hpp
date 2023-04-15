#pragma once

#include <Arduino.h>
#include <UbxGpsNavPvt.h>
#include <UbxGpsConfig.h>
#include "../common.h"


namespace GpsInterface { 

  void init(int tx, int rx);
  void loop();
  
  template <class HardwareSerial>
  UbxGpsNavPvt<HardwareSerial> getGps();

  long getSpeed();
  long getHeading();
  long getHeight();
  long getFallSpeed();
  long getAcceleration();
  bool isFalling();
  DataPoint getStartDp();
  DataPoint getCurDp();
  long getExitTs();

}