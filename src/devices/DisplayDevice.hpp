#pragma once

#include <Arduino.h>

class DisplayDevice
{
public:
  virtual void init() = 0;
  virtual void displayDigits(int current, int previous) = 0;
  virtual void displayString(char* string, int x, int y) = 0;
  virtual void displayString(char* string, int color, int x, int y) = 0;
  virtual void displaySide() = 0;
  virtual void displayTop() = 0;
  virtual void clearScreen() = 0;
  virtual void waitForFrame() = 0;
  virtual void setRotation(int r) = 0;
  virtual void drawRect(int x,int y, int h, int w, int  c ) = 0;
  virtual void drawLine(int x,int y, int x1, int y1, int  c ) = 0;
  virtual void drawArrow(int center_x, int center_y, int angle) = 0;
};