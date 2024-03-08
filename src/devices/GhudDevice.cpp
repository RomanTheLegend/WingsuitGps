#include "DisplayDevice.hpp"
#include "GhudDevice.hpp"
#include "../config.h"

namespace GhudDevice
{

    DisplayDevice *const ghudDevice = new ActiveLookDisplay();

    void init()
    {
        ghudDevice->init();
    }

    void clearScreen()
    {
        ghudDevice->clearScreen();
    }

    void setFontId(int id)
    {
        ghudDevice->setFontId(id);
    }

    void setColor(int id)
    {
        ghudDevice->setColor(id);
    }

    void displayIcon(int id, int x, int y)
    {
        ghudDevice->displayIcon(id, x, y);
    }

    void displayDigits(int current, int previous)
    {
        ghudDevice->displayDigits(current, previous);
    }

    void displayDigits(int current)
    {
        ghudDevice->displayDigits(current);
    }

    void displayString(int num, int x, int y)
    {
        ghudDevice->displayString(num, x, y);
    }

    void displayString(char *string, int x, int y)
    {
        ghudDevice->displayString(string, x, y);
    }

    void displayString(std::string data, int x, int y)
    {
        ghudDevice->displayString(data, x, y);
    }

    void displayString(std::string data, int color, int x, int y)
    {
        ghudDevice->displayString(data, color, x, y);
    }

    void displayString(std::string data, int size, int color, int x, int y)
    {
        ghudDevice->displayString(data, size, color, x, y);
    }

    void drawRect(int x, int y, int h, int w, int c)
    {
        ghudDevice->drawRect(x, y, h, w, c);
    }

    void drawLine(int x, int y, int x1, int y1, int c)
    {
        ghudDevice->drawLine(x, y, x1, y1, c);
    }

    void drawArrow(int cX, int cY, int angle)
    {
        ghudDevice->drawArrow(cX, cY, angle);
    }

    void setRotation(int r)
    {
        ghudDevice->setRotation(r);
    }

    void waitForFrame()
    {
        ghudDevice->waitForFrame();
    }

    void releaseFrame()
    {
        ghudDevice->releaseFrame();
    }

}