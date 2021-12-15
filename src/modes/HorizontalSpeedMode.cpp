#include "../devices/GhudDevice.hpp"
#include "../modules/ButtonInterface.hpp"
#include "../modules/GpsInterface.hpp"
#include "HorizontalSpeedMode.hpp"

int hSpeedPrevious; 

void HorizontalSpeedMode::init(){
  GhudDevice::clearScreen();  
  hSpeedPrevious = -1;
}


void HorizontalSpeedMode::processInput(ButtonEvent event){
  
}


void HorizontalSpeedMode::display(){
  float fSpeed = GpsInterface::getGps()->f_speed_kmph();
  int hSpeed = int(fSpeed);  

  GhudDevice::displayDigits(hSpeed, hSpeedPrevious);
  hSpeedPrevious=hSpeed;  
}