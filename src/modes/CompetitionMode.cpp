#include "../devices/GhudDevice.hpp"
#include "../modules/ButtonInterface.hpp"
#include "../modules/GpsInterface.hpp"
#include "DisplayMode.hpp"

class CompetitionMode : public DisplayMode
{

public:

  void init()
  {
    comp_mode_random_speed = 240;
    GhudDevice::clearScreen();
  }

  void processInput(ButtonEvent event)
  {
  }

  void display()
  {

    getRandomSpeed();
    long height = GpsInterface::getHeight();

    int angle = millis()/100 % 360;
    if (angle != prev_angle){
      GhudDevice::clearScreen();
      prev_angle = angle;
    }
    GhudDevice::waitForFrame();
    GhudDevice::displayString("Alt", 0x53, 30, 0);
    GhudDevice::displayString("3619", 0x77, 120, 0);
    GhudDevice::drawLine(11, 40, 233, 40, 0x6C);

    // GhudDevice::setFontId(2);
    GhudDevice::displayDigits(comp_mode_random_speed, comp_mode_prev_random);

    // GhudDevice::drawRect(11, 150, 223, 65, 0x6C);
    GhudDevice::drawLine(11, 150, 233, 150, 0x6C);

    // GhudDevice::drawArrow(100, 140, 90);

    if (GpsInterface::detectFreefall())
    {
      checkLane();
    } else {
      GhudDevice::displayString("Not in freefall", 0x77, 30, 180);
      GhudDevice::drawArrow(120, 175, angle);

    }

    comp_mode_prev_random = comp_mode_random_speed;
  }

private:

  DataPoint targetDp;

  int comp_mode_random_speed = 240;
  int comp_mode_prev_random = 0;
  int prev_angle = 0;

  void getRandomSpeed()
  {
    int randNumber = random(3);
    if (comp_mode_random_speed < randNumber)
    {
      comp_mode_random_speed = comp_mode_random_speed + randNumber;
    }
    else
    {
      int direction = random(4);
      if (direction > 1)
      {
        comp_mode_random_speed = comp_mode_random_speed + randNumber;
      }
      else
      {
        comp_mode_random_speed = comp_mode_random_speed - randNumber;
      }
    }
  }

  void checkLane()
  {
    DataPoint curDp = GpsInterface::getCurDp();
    if (GpsInterface::getExitTs() == 0){
      GhudDevice::displayString(" Ololo ", 0x77, 100, 140);

    }
    else{
      int secondsSinceExit = int ( (curDp.ts -  GpsInterface::getExitTs()) / 1000 );
      GhudDevice::displayString("Lane lock in ", 0x77, 30, 180);
      GhudDevice::displayString("9", 0x77, 190, 180); // 10 - secondsSinceExit
      GhudDevice::drawArrow(130, 175, 0);
    }

  }
};
