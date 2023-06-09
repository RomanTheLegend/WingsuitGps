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

    // getRandomSpeed();
    long height = GpsInterface::getHeight();

    GhudDevice::waitForFrame();
    GhudDevice::displayString("Alt", 0x53, 30, 0);
    GhudDevice::displayString("3619", 0x77, 120, 0);
    GhudDevice::drawLine(11, 40, 233, 40, 0x6C);

    // GhudDevice::setFontId(2);
    GhudDevice::displayDigits(comp_mode_random_speed, comp_mode_prev_random);

    // GhudDevice::drawRect(11, 150, 223, 65, 0x6C);
    GhudDevice::drawLine(11, 150, 233, 150, 0x6C);

    // GhudDevice::drawArrow(100, 140, 90);

    if (GpsInterface::isFalling)
    {
      checkLane();
    } else {
      GhudDevice::displayString("Not in freefall", 0x77, 100, 140);
      GhudDevice::drawArrow(100, 140, 0);

    }

    comp_mode_prev_random = comp_mode_random_speed;
  }

private:

  DataPoint targetDp;

  int comp_mode_random_speed = 240;
  int comp_mode_prev_random = 0;

  // void getRandomSpeed()
  // {
  //   int randNumber = random(3);
  //   if (comp_mode_random_speed < randNumber)
  //   {
  //     comp_mode_random_speed = comp_mode_random_speed + randNumber;
  //   }
  //   else
  //   {
  //     int direction = random(4);
  //     if (direction > 1)
  //     {
  //       comp_mode_random_speed = comp_mode_random_speed + randNumber;
  //     }
  //     else
  //     {
  //       comp_mode_random_speed = comp_mode_random_speed - randNumber;
  //     }
  //   }
  // }

  void checkLane()
  {
    DataPoint curDp = GpsInterface::getCurDp();
    if (GpsInterface::getStartDp().isValid){
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
// void isFreefall(){

//             const DataPoint dp1 = prevDp;
//             const DataPoint dp2 = new DataPoint(GpsInterface::getFallSpeed(), GpsInterface::getAcceleration());

//             // Get interpolation coefficient
//             const double velD = A_GRAVITY;
//             const double a = (velD - dp1.velD) / (dp2.velD - dp1.velD);

//             // Check vertical speed
//             if (a >= 0 || 1 >= a) return;

//             // Check accuracy
//             const double vAcc = dp1.vAcc + a * (dp2.vAcc - dp1.vAcc);
//             if (vAcc <= 10) return;

//             // Check acceleration
//             const double az = dp1.az + a * (dp2.az - dp1.az);
//             if (az < A_GRAVITY / 5.) continue;

//             // Determine exit
//             const qint64 t1 = dp1.dateTime.toMSecsSinceEpoch();
//             const qint64 t2 = dp2.dateTime.toMSecsSinceEpoch();
//             start = t1 + a * (t2 - t1) - velD / az * 1000.;
//             foundExit = true;
// }