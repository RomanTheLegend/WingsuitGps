#pragma once
#include "../devices/GhudDevice.hpp"
#include "../modules/ButtonInterface.hpp"
#include "../modules/GpsInterface.hpp"
#include "DisplayMode.hpp"

class HeadingMode : public DisplayMode
{
long previousHeading; 

void init(){
  GhudDevice::clearScreen();  
  previousHeading = -1;
}


void processInput(ButtonEvent event){
  
}


void display(){
  long heading = GpsInterface::getHeading(); 

  GhudDevice::displayDigits(heading, previousHeading);
  previousHeading=heading;
}
};