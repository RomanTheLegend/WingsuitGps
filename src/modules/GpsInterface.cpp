#include <Arduino.h>
#include <HardwareSerial.h>
#include <SPI.h>
#include <Wire.h>
#include "GpsInterface.hpp"
#include "BluetoothInterface.hpp"
#include "../common.h"

// #define GPS_DEBUG

namespace GpsInterface
{

#define SLOPE_LEN 4

  HardwareSerial SerialGPS(2);
  UbxGpsNavPvt<HardwareSerial> gps(SerialGPS);

#define GPS_BAUDRATE 115200

#define DATETIME_FORMAT "%04d.%02d.%02d %02d:%02d:%02d"
#define DATETIME_LENGTH 20
  char datetime[DATETIME_LENGTH];

  DataPoint timeseries[SLOPE_LEN];

  DataPoint curDp;
  long long exitTs = 0;
  long long firstSignalTs = 0;

  bool alreadyFalling = false;

  void addElement(DataPoint value, DataPoint array[], int size)
  {
    for (int i = 0; i < size - 1; ++i)
    {
      array[i] = array[i + 1];
    }
    array[size - 1] = value;
  }

  double getSlope()
  {
    double sumx = 0, sumy = 0, sumxx = 0, sumxy = 0;

    for (int i = 0; i <= SLOPE_LEN - 1; ++i)
    {
      double y = timeseries[i].velD;
      double t = timeseries[i].t;

      sumx += t;
      sumy += y;
      sumxx += t * t;
      sumxy += t * y;
    }

    int n = SLOPE_LEN;
    double slope = (sumxy - sumx * sumy / n) / (sumxx - sumx * sumx / n);
    return slope;
  }

  void init(int tx, int rx)
  {

    UbxGpsConfig<HardwareSerial, HardwareSerial> *ubxGpsConfig = new UbxGpsConfig<HardwareSerial, HardwareSerial>(SerialGPS, Serial);
    ubxGpsConfig->setCustomPins(tx, rx);
    ubxGpsConfig->setBaudrate(GPS_BAUDRATE);
    ubxGpsConfig->setMessage(UbxGpsConfigMessage::NavPvt);
    ubxGpsConfig->setRate(200);
    ubxGpsConfig->configure();
    delete ubxGpsConfig;

    gps.begin(GPS_BAUDRATE, SERIAL_8N1, tx, rx);

    Serial.println("GPS interface initialized");
  }

  void loop()
  {

    if (gps.ready())
    {
#ifdef GPS_DEBUG
      snprintf(datetime, DATETIME_LENGTH, DATETIME_FORMAT, gps.year, gps.month, gps.day, gps.hour, gps.min, gps.sec);

      Serial.print(datetime);
      Serial.print(", Lon: ");
      Serial.print(gps.lon / 10000000.0, 7);
      Serial.print(" , Lat: ");
      Serial.print(gps.lat / 10000000.0, 7);
      Serial.print(" , Height: ");
      Serial.print(gps.height / 1000.0, 3);
      Serial.print(" , Speed: ");
      Serial.print(gps.gSpeed * 0.0036, 5);
      Serial.print(" , Heading: ");
      Serial.print(gps.heading / 100000.0, 5);
      Serial.print(" , Fix type: ");
      Serial.print(gps.fixType);
      Serial.print(" , Sats: ");
      Serial.println(gps.numSV);
#endif
      curDp.vAcc = getAcceleration();
      curDp.velD = getFallSpeed();
      curDp.ts = gps.iTOW;

      if (firstSignalTs != 0)
      {
        curDp.t = (double)(curDp.ts - firstSignalTs) / 1000;
        addElement(curDp, timeseries, SLOPE_LEN);
      }
      else
      {
        firstSignalTs = gps.iTOW;
      }

      if (!alreadyFalling){
        detectFreefall();
      }

      // if (!runStartDp.isValid && (gps.iTOW - exitTs > 10000))
      // {
      //   runStartDp.lat = gps.lat;
      //   runStartDp.lon = gps.lon;
      //   runStartDp.ts = gps.iTOW;
      //   runStartDp.isValid = true;
      // }
    }
    // while (SerialGPS.available())
    // {
    //   char c = SerialGPS.read();
    //   BluetoothInterface::sendData(c);
    //   //Serial.write(c); // uncomment this line if you want to see the GPS data flowing
    //   if (gps.encode(c)) // Did a new valid sentence come in?
    //     newData = true;
    // }
  }

  // template <class HardwareSerial>
  // UbxGpsNavPvt<HardwareSerial> getGps()
  // {
  //     return gps;
  // }

  long getSpeed()
  {
    return long(gps.gSpeed * 0.0036);
  }

  long getHeading()
  {
    return long(gps.heading / 100000.0 + 0.5);
  }

  long getHeight()
  {
    return long(gps.height / 1000.0 + 0.5);
  }

  bool detectFreefall()
  {
    if (alreadyFalling)
      return true;

    // Get acceleration using linear least squares
    timeseries[SLOPE_LEN - 1].az = getSlope();
    double g = A_GRAVITY;

    int p = SLOPE_LEN - 2, c = SLOPE_LEN - 1;

    double a = (g - timeseries[p].velD) / (timeseries[c].velD - timeseries[p].velD);
    // Check vertical speed
    if (a < 0 || 1 < a)
      return false;

    // Check accuracy
    double vAcc = timeseries[p].vAcc + a * (timeseries[c].vAcc - timeseries[p].vAcc);
    if (vAcc > 10)
      return false;

    // Check acceleration
    double az = timeseries[p].az + a * (timeseries[c].az - timeseries[p].az);
    if (az < g / 5.)
      return false;

    exitTs = (long long)(timeseries[p].ts + a * (timeseries[c].ts - timeseries[p].ts) - g / az * 1000.);
    alreadyFalling = true;
    return alreadyFalling;
  }

  // DataPoint getStartDp()
  // {
  //   return runStartDp;
  // }

  long getExitTs()
  {
    return exitTs;
  }

  DataPoint getCurDp()
  {
    return curDp;
  }

  long getFallSpeed()
  {
    return long(gps.velD * 0.0036);
  }

  long getAcceleration()
  {
    return long(gps.vAcc * 0.0036);
  }

}