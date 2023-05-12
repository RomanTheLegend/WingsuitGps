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

  HardwareSerial SerialGPS(2);

  UbxGpsNavPvt<HardwareSerial> gps(SerialGPS);

#define GPS_BAUDRATE 115200

#define DATETIME_FORMAT "%04d.%02d.%02d %02d:%02d:%02d"
#define DATETIME_LENGTH 20
  char datetime[DATETIME_LENGTH];



  DataPoint curDp;
  DataPoint prevDp;
  DataPoint runStartDp;
  long exitTs;

  bool alreadyFalling = false;

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
      prevDp = curDp;
      curDp.vAcc = getAcceleration();
      curDp.velD = getFallSpeed();
      curDp.ts = gps.iTOW;

      if (!runStartDp.isValid && (gps.iTOW -  exitTs > 10000)) {
        runStartDp.lat = gps.lat;
        runStartDp.lon = gps.lon;
        runStartDp.ts = gps.iTOW;
        runStartDp.isValid = true;
      }
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

  bool isFreefall()
  {
    if (alreadyFalling)
      return true;

    // Get interpolation coefficient
    double a = (A_GRAVITY - prevDp.velD) / (curDp.velD - prevDp.velD);

    // Check vertical speed
    if (a < 0 || 1 < a) return false;

    // Check accuracy
    double vAcc = prevDp.vAcc + a * (curDp.vAcc - prevDp.vAcc);
    if (vAcc > 10) return false;

    // Check acceleration
    double az = prevDp.az + a * (curDp.az - prevDp.az);
    if (az < A_GRAVITY / 5.) return false;

    exitTs= prevDp.ts + a * (curDp.ts - prevDp.ts) - A_GRAVITY / az * 1000.;
    alreadyFalling = true;
    return true;
  }

  DataPoint getStartDp(){
    return runStartDp;
  }

  long getExitTs(){
    return exitTs;
  }

  DataPoint getCurDp(){ 
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