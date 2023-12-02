#include "../devices/GhudDevice.hpp"
#include "../modules/ButtonInterface.hpp"
#include "DisplayMode.hpp"

class RandomSpeedMode : public DisplayMode
{

private:
volatile int random_speed;
int prev_random=0;

public: 

void init(){
  random_speed = 240;
  GhudDevice::clearScreen();
}



void processInput(ButtonEvent event){
  
}


void display(){

    int randNumber = random(3);
    if (random_speed < randNumber){
        random_speed = random_speed + randNumber;
    } else {
      int direction =  random(4);
      if (direction > 1){
        random_speed = random_speed + randNumber;        
      } else {
        random_speed = random_speed - randNumber;
      }
    }
  
  GhudDevice::waitForFrame();
  GhudDevice::displayDigits(random_speed, prev_random);
  prev_random=random_speed;
}
};