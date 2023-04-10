#include "../devices/GhudDevice.hpp"
#include "../modules/ButtonInterface.hpp"
#include "RectanglesMode.hpp"

long fractionSecond;
uint8_t currentState;
uint8_t delta;

void RectanglesMode::init(){
  currentState = 0;
  fractionSecond = millis()%1000;
  delta = 0;
}



void RectanglesMode::processInput(ButtonEvent event){
  
}


void RectanglesMode::display(){
  if (millis()%1000 < fractionSecond)
  {
    // Cycle through one of four states every second
    currentState = (currentState+1)%4;
  }
  fractionSecond = millis()%1000;

  // Calculate the fractional progress through animated delta value
  delta = fractionSecond/5;
  if(delta > 100)
  {
    delta = 200-delta;
  }
  
  
  GhudDevice::waitForFrame();
  GhudDevice::clearScreen();

  for(uint8_t r = 0; r < 4; r++)
  {
    GhudDevice::setRotation(r);
    switch(currentState)
    {
      case 0:
        GhudDevice::drawRect(40+delta, 40, 20, 20, 0xFF);
        break;
      case 1:
        GhudDevice::drawRect(40, 40+delta, 20, 20, 0xFF);
        break;
      case 2:
        GhudDevice::drawRect(40, 40, 20+delta, 20, 0xFF);
        break;
      case 3:
        GhudDevice::drawRect(40, 40, 20, 20+delta, 0xFF);
        break;
      default:
        currentState = 0;
        break;
    }
  }
}