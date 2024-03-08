#pragma once
#include <Arduino.h>
#include <string>

class DisplayDevice
{
public:
    virtual void init() = 0;
    virtual void clearScreen() = 0;
    virtual void setFontId(int id) = 0;
    virtual void setColor(int id) = 0;
    virtual void displayIcon(int id, int x, int y) = 0;
    virtual void displayDigits(int current, int previous) = 0;
    virtual void displayDigits(int current) = 0;
    virtual void displayString(int num, int x, int y) = 0;
    virtual void displayString(char *string, int x, int y) = 0;
    virtual void displayString(std::string data, int x, int y) = 0;
    virtual void displayString(std::string data, int color, int x, int y) = 0;
    virtual void displayString(std::string data, int size, int color, int x, int y) = 0;
    virtual void drawRect(int x, int y, int h, int w, int c) = 0;
    virtual void drawLine(int x, int y, int x1, int y1, int c) = 0;
    virtual void drawArrow(int cX, int cY, int angle) = 0;
    virtual void setRotation(int r) = 0;
    virtual void waitForFrame() = 0;
    virtual void releaseFrame() = 0;
};