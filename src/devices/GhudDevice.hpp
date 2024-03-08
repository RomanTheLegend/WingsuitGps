#include <string>

namespace GhudDevice {
    void init();
    void clearScreen();
    void setFontId(int id);
    void setColor(int id);
    void displayIcon(int id, int x, int y);
    void displayDigits(int current, int previous);
    void displayDigits(int current);
    void displayString(int num, int x, int y);
    void displayString(char *string, int x, int y);
    void displayString(std::string data, int x, int y);
    void displayString(std::string data, int color, int x, int y);
    void displayString(std::string data, int size, int color, int x, int y);
    void drawRect(int x, int y, int h, int w, int c);
    void drawLine(int x, int y, int x1, int y1, int c);
    void drawArrow(int cX, int cY, int angle);
    void setRotation(int r);
    void waitForFrame();
    void releaseFrame();
};