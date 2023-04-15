#pragma once

#include "DisplayDevice.hpp"


class CompositeDisplay : public DisplayDevice
{
  public:
    void init() override;
    void displayDigits(int current, int previous) override;
    void displayString(char* string, int x, int y) override;
    void displayString(char* string, int color, int x, int y) override;
    void displaySide() override;
    void displayTop() override;
    void clearScreen() override;
    void waitForFrame() override;
    void setRotation(int r) override;
    void drawRect(int x,int y, int h, int w, int  c ) override;
    void drawLine(int x,int y, int x1, int y1, int  c ) override;
    void drawArrow(int center_x, int center_y, int angle) override;
    void setFont(int id);
    void setColor(int id);
};