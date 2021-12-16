#pragma once

#include "BluetoothSerial.h"

namespace BluetoothInterface {
  void init();
  void sendData(String data);
  void sendData(char c);
}