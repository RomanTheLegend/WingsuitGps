


namespace GhudDevice {
    void init();
    void displayDigits(int current, int previous);
    void displayString(char* string, int x, int y);
    void displaySide();
    void displayTop();
    void clearScreen();
    void waitForFrame();
    void setRotation(int r);
    void drawRect(int x,int y, int h, int w, int  c );
};