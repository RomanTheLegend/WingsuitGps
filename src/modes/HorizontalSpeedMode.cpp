#include "../devices/GhudDevice.hpp"
#include "../modules/ButtonInterface.hpp"
#include "../modules/DataProvider.cpp"
#include <HardwareSerial.h>
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
  DataPoint dp = DataProvider::getInstance().getDataPoint();
  long hSpeed = long (sqrt(dp.velN * dp.velN + dp.velE * dp.velE));

  // Serial.print(" hSpeed=");Serial.print(hSpeed);
  // Serial.print(" velN=");Serial.print(dp.velN);
  // Serial.print(" velE=");Serial.println(curDp.velE);
  GhudDevice::waitForFrame();
  GhudDevice::displayDigits(hSpeed, hSpeedPrevious);
  hSpeedPrevious=hSpeed;  
}
};
