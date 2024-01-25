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
  int hMSL = 0;
  char alt[4];

  // void getRandomSpeed()
  // {
  //   int randNumber = random(3);
  //   if (horizontal_speed < randNumber)
  //   {
  //     horizontal_speed = horizontal_speed + randNumber;
  //   }
  //   else
  //   {
  //     int direction = random(4);
  //     if (direction > 1)
  //     {
  //       horizontal_speed = horizontal_speed + randNumber;
  //     }
  //     else
  //     {
  //       horizontal_speed = horizontal_speed - randNumber;
  //     }
  //   }
  // }

  double haversine(double lat1, double lon1, double lat2, double lon2)
  {
    const double R = 6371000.0; // Earth radius in meters

    double dlat = (lat2 - lat1) * M_PI / 180.0;
    double dlon = (lon2 - lon1) * M_PI / 180.0;

    double a = sin(dlat / 2.0) * sin(dlat / 2.0) +
               cos(lat1 * M_PI / 180.0) * cos(lat2 * M_PI / 180.0) *
                   sin(dlon / 2.0) * sin(dlon / 2.0);

    double c = 2.0 * atan2(sqrt(a), sqrt(1.0 - a));

    return R * c;
  }

  bool isLeft(double startLat, double startLon, double finishLat, double finishLon,
              double positionLat, double positionLon)
  {
    // Vectors representing the line and the vector from start to position
    double lineVectorX = finishLon - startLon;
    double lineVectorY = finishLat - startLat;
    double positionVectorX = positionLon - startLon;
    double positionVectorY = positionLat - startLat;

    // Calculate the cross product
    double cross = lineVectorX * positionVectorY - positionVectorX * lineVectorY;

    // Determine the orientation based on the sign of the cross product
    return cross > 0.0;
  }

  void checkLane(DataPoint dp)
  {
    int secondsSinceExit = int((dp.ts - DataProvider::getInstance().getExitTs()) / 1000);
    Serial.print("dp.ts: ");Serial.print(dp.ts); Serial.print(" exitTs: ");Serial.print(DataProvider::getInstance().getExitTs());Serial.print(" secondsSinceExit: ");Serial.println(secondsSinceExit);
    if (secondsSinceExit >= 10)
    {
      DataPoint exitDp = DataProvider::getInstance().getExitDp();
      int dist = (int)(haversine(exitDp.lat, exitDp.lon, dp.lat, dp.lon) +
                       haversine(dp.lat, dp.lon, targetDp.lat, targetDp.lon) -
                       haversine(exitDp.lat, exitDp.lon, targetDp.lat, targetDp.lon));

      bool left_side = isLeft(exitDp.lat, exitDp.lon, targetDp.lat, targetDp.lon, dp.lat, dp.lon);

      char c_dist[4];
      char side[1];
      sprintf(c_dist, "%d", dist);
      // sprintf(side, "%c", left_side ? 'L' : 'R');

      GhudDevice::displayString("Deviation: ", 0x77, 30, 180);
      GhudDevice::displayString(c_dist, 0x77, 150, 180);
      // GhudDevice::displayString(side, 0x77, 180, 180);
    }
    else
    {
      GhudDevice::displayString("Lane lock in ", 0x77, 30, 180);
      int lock_countdown = 10 - secondsSinceExit;
      char c_time[2];
      sprintf(c_time, "%d", lock_countdown);
      GhudDevice::displayString(c_time, 0x77, 190, 180);
      // GhudDevice::drawArrow(120, 175, dp.heading);
    }
  }

public:
  void init()
  {
    horizontal_speed = 240;
    targetDp.lat = 49.459110;
    targetDp.lon = 17.099134;
    // targetDp.lat = 49.489993;
    // targetDp.lon = 20.028301;
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
    horizontal_speed = int(sqrt(dp.velN * dp.velN + dp.velE * dp.velE) * 3.6);
    hMSL = int(dp.hMSL);
    // int angle = millis()/100 % 360;
    int angle = int(dp.heading);

    GhudDevice::setFontId(1);

    GhudDevice::displayString("Alt", 0x53, 30, 10);

    sprintf(alt, "%d", hMSL);
    GhudDevice::displayString(alt, 0x77, 120, 10);
    GhudDevice::drawLine(11, 40, 233, 40, 0x6C);

    GhudDevice::displayDigits(horizontal_speed);

    GhudDevice::drawLine(11, 150, 233, 150, 0x6C);

    // GhudDevice::drawArrow(100, 140, 90);

    if (DataProvider::getInstance().getExitTs() != 0)
    {
      checkLane(dp);
    }
    else
    {
      GhudDevice::setFontId(1);
      GhudDevice::displayString("Not in freefall", 0x77, 20, 190);
      GhudDevice::drawArrow(120, 175, angle);
    }
  }
};
