#pragma once

#include <Arduino.h>

class DisplayDevice
{
public:
  virtual void init() = 0;
  virtual void displayDigits(int current, int previous) = 0;
  virtual void displayString(char* string, int x, int y) = 0;
  virtual void displaySide() = 0;
  virtual void displayTop() = 0;
  virtual void clearScreen() = 0;
};