


namespace GhudDevice {
    void init();
    void displayDigits(int current, int previous);
    void displayString(char* string, int x, int y);
    void displayString(char* string, int color, int x, int y);
    void displaySide();
    void displayTop();
    void clearScreen();
    void waitForFrame();
    void setFontId(int r);
    void setRotation(int r);
    void drawRect(int x,int y, int h, int w, int  c );
    void drawLine(int x,int y, int x1, int y1, int  c );
    void drawArrow(int center_x, int center_y, int angle);

};