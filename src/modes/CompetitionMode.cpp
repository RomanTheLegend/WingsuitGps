#include "../devices/GhudDevice.hpp"
#include "../modules/ButtonInterface.hpp"
#include "../modules/DataProvider.cpp"
#include "DisplayMode.hpp"

class CompetitionMode : public DisplayMode
{

private:
  // DataProvider dataProvider;
  DataPoint targetDp;

  int horizontal_speed= 0, vertical_speed= 0, hMSL= 0, gps = 0, countdown = 0;
  int prev_hs= 0, prev_vs= 0, prev_hMSL= 0, prev_gps = -1 , prev_countdown = 0;

  bool refreshScreen=true;

  const char *LOG_TAG = "Competition Mode";

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

      // char side[1] = left_side ? 'L' : 'R';

      GhudDevice::displayString("Deviation: ", 30, 180);
      GhudDevice::displayString(dist, 150, 180);
      // GhudDevice::displayString(side, 180, 180);
    }
    else
    {
      GhudDevice::displayString("Lane lock in", 0, 180);
      int lock_countdown = 10 - secondsSinceExit;
      // GhudDevice::displayString(prev_countdown, 190, 180);
      // GhudDevice::displayString(lock_countdown, 190, 180);
      // if (lock_countdown != prev_countdown){
      //   GhudDevice::displayString(std::to_string(prev_countdown), 2 , 0, 220, 180);
      //   GhudDevice::displayString(std::to_string(lock_countdown), 2 , 1, 220, 180);
      //   prev_countdown = lock_countdown;
      // }
      // GhudDevice::drawArrow(120, 175, dp.heading);
    }
  }

public:
  void init()
  {
    targetDp.lat = 49.459110;
    targetDp.lon = 17.099134;
    // targetDp.lat = 49.489993;
    // targetDp.lon = 20.028301;
    GhudDevice::waitForFrame();
    GhudDevice::clearScreen();


    GhudDevice::displayString("v", 10, 90);
    GhudDevice::displayIcon(48,80,133);

    GhudDevice::displayString("h", 10, 30);
    GhudDevice::displayIcon(48,80,70);

    // Altitude
    GhudDevice::displayIcon(34,50,20);
  }

  void processInput(ButtonEvent event)
  {
  }

  void display()
  {

    GhudDevice::waitForFrame();
    // GhudDevice::clearScreen();

    DataPoint dp = DataProvider::getInstance().getDataPoint();
    horizontal_speed = int(sqrt(dp.velN * dp.velN + dp.velE * dp.velE) * 3.6);
    vertical_speed = int(dp.velD * 3.6);
    hMSL = int(dp.hMSL);
    // int angle = millis()/100 % 360;
    int angle = int(dp.heading);

    // GhudDevice::setFontId(1);

    //V-speed

    if (vertical_speed != prev_vs){
      ESP_LOGD(LOG_TAG, "V-speed");
      GhudDevice::displayString(std::to_string(prev_vs), 3 , 0, 100, 67);
      GhudDevice::displayString(std::to_string(vertical_speed), 3 , 1,  100, 67);
      prev_vs = vertical_speed;
    }
        //H-speed

    if (horizontal_speed != prev_hs){
      ESP_LOGD(LOG_TAG, "H-speed");
      GhudDevice::displayString(std::to_string(prev_hs), 3 , 0, 100, 5);
      GhudDevice::displayString(std::to_string(horizontal_speed), 3 , 1,  100, 5);
      prev_hs = horizontal_speed;
    }


      // GhudDevice::displayIcon(34,50,20);
    if (gps != prev_gps){
      ESP_LOGD(LOG_TAG, "GPS");
      GhudDevice::displayString(std::to_string(dp.numSV), 1, 1, 200, 5);
      GhudDevice::displayString("GPS",  1 , 1, 190, -20);
      prev_gps = gps;
    }


    if (hMSL != prev_hMSL){
      ESP_LOGD(LOG_TAG, "HMSL");
      GhudDevice::displayString(prev_hMSL, 70, -20);
      GhudDevice::displayString(hMSL, 70, -20);
      prev_hMSL = hMSL;
    }

    GhudDevice::drawLine(11, 150, 233, 150, 0x6C);

    // GhudDevice::drawArrow(100, 140, 90);

    if (DataProvider::getInstance().getExitTs() != 0)
    {
      checkLane(dp);
    }
    else
    {
      GhudDevice::setFontId(1);
      // GhudDevice::displayString("Not in freefall", 20, 190);
      GhudDevice::drawArrow(120, 175, angle);
    }
  }
};
