#include "DisplayDevice.hpp"
#include "GhudDevice.hpp"
#include "LiLyGo_TGO.hpp"

namespace GhudDevice {

    DisplayDevice* const ghudDevice = new TGO();

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