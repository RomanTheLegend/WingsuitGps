#pragma once
#include "../devices/GhudDevice.hpp"
#include "../modules/ButtonInterface.hpp"
#include "../modules/DataProvider.cpp"
#include "DisplayMode.hpp"

class HeadingMode : public DisplayMode
{
private:
  long previousHeading; 

public:
void init(){
  GhudDevice::clearScreen();
  previousHeading = -1;
}

void processInput(ButtonEvent event){
  
}


void display(){
  long heading = long(DataProvider::getInstance().getDataPoint().heading); 

  GhudDevice::displayDigits(heading, previousHeading);
  previousHeading=heading;
}
};