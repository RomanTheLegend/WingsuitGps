#include "../devices/GhudDevice.hpp"
#include "../modules/ButtonInterface.hpp"
#include "../modules/DataProvider.cpp"
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
  long hSpeed = long (DataProvider::getInstance().getDataPoint().horizontalSpeed);

  GhudDevice::waitForFrame();
  GhudDevice::displayDigits(hSpeed, hSpeedPrevious);
  hSpeedPrevious=hSpeed;  
}
};
