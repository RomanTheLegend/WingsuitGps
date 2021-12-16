#include "../devices/GhudDevice.hpp"
#include "../modules/ButtonInterface.hpp"
#include "../modules/GpsInterface.hpp"
#include "HeadingMode.hpp"

int previousHeading; 

void HeadingMode::init(){
  GhudDevice::clearScreen();  
  previousHeading = -1;
}


void HeadingMode::processInput(ButtonEvent event){
  
}


void HeadingMode::display(){
  float fHeading = GpsInterface::getGps()->f_course();
  int heading = int(fHeading);  

  GhudDevice::displayDigits(heading, previousHeading);
  previousHeading=heading;
}