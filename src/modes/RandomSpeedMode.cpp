#include "../devices/GhudDevice.hpp"
#include "../modules/ButtonInterface.hpp"
#include "RandomSpeedMode.hpp"

volatile int random_speed;
int prev_random=0;

void RandomSpeedMode::init(){
  random_speed = 240;
  GhudDevice::clearScreen();
}



void RandomSpeedMode::processInput(ButtonEvent event){
  
}


void RandomSpeedMode::display(){

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