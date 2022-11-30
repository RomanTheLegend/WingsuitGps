#include "DisplayDevice.hpp"
#include "GhudDevice.hpp"
#include "../config.h"

namespace GhudDevice {

    DisplayDevice* const ghudDevice = new CompositeDisplay();


    void init(){
        ghudDevice -> init();
    }

    void displayDigits(int current, int previous){
        ghudDevice -> displayDigits( current,  previous);
    }


    void displayString(char* string, int x, int y){
        ghudDevice -> displayString(string,  x,  y);
    }

    void displaySide(){
        ghudDevice -> displaySide();
    }

    void displayTop(){
        ghudDevice -> displayTop();
    }

    void clearScreen(){
        ghudDevice -> clearScreen();
    }

    void waitForFrame(){
        ghudDevice -> waitForFrame();
    }

    void setRotation(int r){
        ghudDevice -> setRotation(r);
    }

    void drawRect(int x,int y, int h, int w, int  c ){
        ghudDevice -> drawRect(x,y,h,w,c);
    }
}