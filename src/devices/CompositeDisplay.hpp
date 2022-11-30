#pragma once

#include "DisplayDevice.hpp"


class CompositeDisplay : public DisplayDevice
{
  public:
    void init() override;
    void displayDigits(int current, int previous) override;
    void displayString(char* string, int x, int y) override;
    void displaySide() override;
    void displayTop() override;
    void clearScreen() override;
    void waitForFrame() override;
    void setRotation(int r) override;
    void drawRect(int x,int y, int h, int w, int  c ) override;
};