#include "DisplayDevice.hpp"
#include "GhudDevice.hpp"
#include "../config.h"

namespace GhudDevice {

    #ifdef USE_TGO
    DisplayDevice* const ghudDevice = new TGO();
    #else
    DisplayDevice* const ghudDevice = new TTV();
    #endif


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
}