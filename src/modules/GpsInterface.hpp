#pragma once

#include <Arduino.h>
#include <TinyGPS.h>

namespace GpsInterface { 

  void init(int tx, int rx);
  void loop();
  TinyGPS* getGps();
}