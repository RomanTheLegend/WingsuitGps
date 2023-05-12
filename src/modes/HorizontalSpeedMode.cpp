#include "../devices/GhudDevice.hpp"
#include "../modules/ButtonInterface.hpp"
#include "../modules/GpsInterface.hpp"
#include "DisplayMode.hpp"

class HorizontalSpeedMode : public DisplayMode
{
private:
int hSpeedPrevious; 

public:
void init(){
  GhudDevice::clearScreen();  
  hSpeedPrevious = -1;
}


void processInput(ButtonEvent event){
  
}


void display(){
  // auto gps = GpsInterface::getGps<HardwareSerial>();
  long hSpeed = GpsInterface::getSpeed();

  GhudDevice::waitForFrame();
  GhudDevice::displayDigits(hSpeed, hSpeedPrevious);
  hSpeedPrevious=hSpeed;  
}
};
