#pragma once

#include <Arduino.h>
#include <UbxGpsNavPvt.h>
#include <UbxGpsConfig.h>

namespace GpsInterface { 

  void init(int tx, int rx);
  void loop();
  
  template <class HardwareSerial>
  UbxGpsNavPvt<HardwareSerial> getGps();

  long getSpeed();
  long getHeading();

}