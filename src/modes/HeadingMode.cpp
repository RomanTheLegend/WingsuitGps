#include "../devices/GhudDevice.hpp"
#include "../modules/ButtonInterface.hpp"
#include "../modules/GpsInterface.hpp"
#include "HeadingMode.hpp"

long previousHeading; 

void HeadingMode::init(){
  GhudDevice::clearScreen();  
  previousHeading = -1;
}


void HeadingMode::processInput(ButtonEvent event){
  
}


void HeadingMode::display(){
  // float fHeading = 0;// GpsInterface::getGps()->f_course();
  long heading = GpsInterface::getHeading(); 

  GhudDevice::displayDigits(heading, previousHeading);
  previousHeading=heading;
}