#include "../devices/GhudDevice.hpp"
#include "../modules/ButtonInterface.hpp"
#include "../modules/DataProvider.cpp"
#include "DisplayMode.hpp"

class CompetitionMode : public DisplayMode
{

private:
  // DataProvider dataProvider;
  DataPoint targetDp;

  int horizontal_speed = 240;
  int horizontal_speed_prev = 0;
  int hMSL = 0;
  int prev_hMSL = 0;
  int prev_angle = 0;
  char alt[4];
  char prev_alt[4];

  void getRandomSpeed()
  {
    int randNumber = random(3);
    if (horizontal_speed < randNumber)
    {
      horizontal_speed = horizontal_speed + randNumber;
    }
    else
    {
      int direction = random(4);
      if (direction > 1)
      {
        horizontal_speed = horizontal_speed + randNumber;
      }
      else
      {
        horizontal_speed = horizontal_speed - randNumber;
      }
    }
  }

  void checkLane()
  {
    DataPoint curDp = DataProvider::getInstance().getDataPoint();
    if (DataProvider::getInstance().getExitTs() == 0){
      GhudDevice::displayString(" Ololo ", 0x77, 100, 140);

    }
    else{
      int secondsSinceExit = int ( (curDp.ts -  DataProvider::getInstance().getExitTs()) / 1000 );
      GhudDevice::displayString("Lane lock in ", 0x77, 30, 180);
      GhudDevice::displayString("9", 0x77, 190, 180); // 10 - secondsSinceExit
      GhudDevice::drawArrow(130, 175, 0);
    }

  }

  public:

  void init()
  {
    horizontal_speed = 240;
    // dataProvider = DataProvider::getInstance();
    GhudDevice::waitForFrame();
    GhudDevice::clearScreen();
  }

  void processInput(ButtonEvent event)
  {
  }

  void display()
  {

    // getRandomSpeed();

    GhudDevice::waitForFrame();
    GhudDevice::clearScreen();

    DataPoint dp = DataProvider::getInstance().getDataPoint();
    horizontal_speed = int (sqrt(dp.velN * dp.velN + dp.velE * dp.velE)*3.6);
    hMSL = int(dp.hMSL);
    // int angle = millis()/100 % 360;
    int angle = int(dp.heading);
    // if (angle != prev_angle){
    //   GhudDevice::clearScreen();
    //   prev_angle = angle;
    // }
    // GhudDevice::waitForFrame();

    GhudDevice::setFontId(1);

    GhudDevice::displayString("Alt", 0x53, 30, 10);

    sprintf(alt, "%d", hMSL);
    // if (prev_hMSL != hMSL){
    //   sprintf(prev_alt, "%d", prev_hMSL );
    //   GhudDevice::displayString(prev_alt, 0x00, 120, 0);
    // }
    GhudDevice::displayString(alt, 0x77, 120, 10);
    GhudDevice::drawLine(11, 40, 233, 40, 0x6C);

    // GhudDevice::setFontId(2);
    // GhudDevice::displayDigits(horizontal_speed, horizontal_speed_prev);
    GhudDevice::displayDigits(horizontal_speed);

    // GhudDevice::drawRect(11, 150, 223, 65, 0x6C);
    GhudDevice::drawLine(11, 150, 233, 150, 0x6C);

    // GhudDevice::drawArrow(100, 140, 90);

    if (DataProvider::getInstance().getExitTs() != 0)
    {
      checkLane();
    } else {
      GhudDevice::setFontId(1);
      GhudDevice::displayString("Not in freefall", 0x77, 20, 190);
      GhudDevice::drawArrow(120, 175, angle);

    }

    horizontal_speed_prev = horizontal_speed;
  }
};
