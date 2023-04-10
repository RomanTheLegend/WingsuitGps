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
  // auto gps = GpsInterface::getGps<HardwareSerial>();
  long hSpeed = GpsInterface::getSpeed();

  GhudDevice::waitForFrame();
  GhudDevice::displayDigits(hSpeed, hSpeedPrevious);
  hSpeedPrevious=hSpeed;  
}