#include "../devices/GhudDevice.hpp"
#include "../modules/ButtonInterface.hpp"
#include "DisplayMode.hpp"

 char* palette[] = {
  "TFT_BLACK",    //  0  ^
  "TFT_BROWN",    //  1  |
  "TFT_RED",      //  2  |
  "TFT_ORANGE",   //  3  |
  "TFT_YELLOW",   //  4  Colours 0-9 follow the resistor colour code!
  "TFT_GREEN",    //  5  |
  "TFT_BLUE",     //  6  |
  "TFT_PURPLE",   //  7  |
  "TFT_DARKGREY", //  8  |
  "TFT_WHITE",    //  9  v
  "TFT_CYAN",     // 10  Blue+green mix
  "TFT_MAGENTA",  // 11  Blue+red mix
  "TFT_MAROON",   // 12  Darker red colour
  "TFT_DARKGREEN",// 13  Darker green colour
  "TFT_NAVY",     // 14  Darker blue colour
  "TFT_PINK"      // 15
};

class FontTest : public DisplayMode
{

private:
volatile int font_random_speed;
int font_prev_random=0;

int font_position = 0;



public:

void init(){
  font_random_speed = 240;
  GhudDevice::clearScreen();
}



void processInput(ButtonEvent event){
  switch (event) {
  case DOWN:
      Serial.println(font_position);    
      if (font_position < 15){
        font_position++;
      } else {
        font_position=0;
      }
    GhudDevice::clearScreen();
    break;
  case UP:
      if (font_position > 0){
        font_position--;
      } else {
        font_position=15;
      }
     GhudDevice::clearScreen();
     break;
}
}

void display(){

  
  
  GhudDevice::waitForFrame();
  // GhudDevice::displayString(palette[font_position], font_position ,20, 30);
  GhudDevice::displayDigits(font_random_speed, font_prev_random);
  font_prev_random=font_random_speed;
}
};