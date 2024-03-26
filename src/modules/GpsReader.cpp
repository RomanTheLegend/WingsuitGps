#pragma once
#include <Arduino.h>
#include <HardwareSerial.h>
#include <SPI.h>
#include <Wire.h>
#include "GpsInterface.hpp"
// #include "BluetoothInterface.hpp"
#include <UbxGpsNavPvt.h>
#include <UbxGpsConfig.h>
#include <TimeLib.h>
#include "../common.h"
#include "config.h"

#define GPS_DEBUG
#define GPS_BAUDRATE 115200

#define DATETIME_FORMAT "%04d.%02d.%02d %02d:%02d:%02d"
#define DATETIME_LENGTH 20

class GpsReader : public GpsInterface
{
private:
  const char *LOG_TAG = "GpsReader";

  DataPoint curDp;
  DataPoint _curDp;

  // HardwareSerial &port;
  UbxGpsNavPvt<HardwareSerial> *ubx_gps;

  char datetime[DATETIME_LENGTH];

  // static GpsReader& getInstanceImpl(HardwareSerial &p)
  // {
  //   static GpsReader instance{ p };
  //   return instance;
  // }

public:
  void refresh()
  {
    if (ubx_gps->ready())
    {
#ifdef GPS_DEBUG
      snprintf(datetime, DATETIME_LENGTH, DATETIME_FORMAT, ubx_gps->year, ubx_gps->month, ubx_gps->day, ubx_gps->hour, ubx_gps->min, ubx_gps->sec);

      Serial.print(datetime);
      Serial.print(", Lon: ");
      Serial.print(ubx_gps->lon / 10000000.0, 7);
      Serial.print(" , Lat: ");
      Serial.print(ubx_gps->lat / 10000000.0, 7);
      Serial.print(" , Height: ");
      Serial.print(ubx_gps->height / 1000.0, 3);
      Serial.print(" , Speed: ");
      Serial.print(ubx_gps->gSpeed * 0.0036, 5);
      Serial.print(" , Heading: ");
      Serial.print(ubx_gps->heading / 100000.0, 5);
      Serial.print(" , Fix type: ");
      Serial.print(ubx_gps->fixType);
      Serial.print(" , Sats: ");
      Serial.println(ubx_gps->numSV);
#endif
      TimeElements tm;
      tm.Year = ubx_gps->year;
      tm.Month = ubx_gps->month;
      tm.Day = ubx_gps->day;
      tm.Hour = ubx_gps->hour;
      tm.Minute = ubx_gps->min;
      tm.Second = ubx_gps->sec;
      int milis = ubx_gps->nano > 0 ? int(ubx_gps->nano / 1000) : 0;
      long long t = makeTime(tm);

      _curDp.velD = ubx_gps->velD * 0.0036;
      _curDp.velE = ubx_gps->velE * 0.0036;
      _curDp.velN = ubx_gps->velN * 0.0036;
      _curDp.ts = t * 1000 + milis;
      _curDp.lat = ubx_gps->lat / 10000000.0;
      _curDp.lon = ubx_gps->lon / 10000000.0;
      _curDp.hMSL = ubx_gps->hMSL / 1000.0;
      _curDp.heading = ubx_gps->heading / 100000.0;
      _curDp.numSV = ubx_gps->numSV;

      curDp = _curDp;
    }
    // else{
    //   ESP_LOGD(LOG_TAG, "GPS device not ready");
    // }
    // while (SerialGPS.available())
    // {
    //   char c = SerialGPS.read();
    //   BluetoothInterface::sendData(c);
    //   //Serial.write(c); // uncomment this line if you want to see the GPS data flowing
    //   if (gps.encode(c)) // Did a new valid sentence come in?
    //     newData = true;
    // }
  }

  DataPoint getLatestData()
  {
    return curDp;
  }

  void init(HardwareSerial &port)
  {
    ubx_gps = new UbxGpsNavPvt<HardwareSerial>(port);
    UbxGpsConfig<HardwareSerial, HardwareSerial> *ubxGpsConfig = new UbxGpsConfig<HardwareSerial, HardwareSerial>(port, Serial);
    ubxGpsConfig->setCustomPins(TX_PIN, RX_PIN);
    ubxGpsConfig->setBaudrate(GPS_BAUDRATE);
    ubxGpsConfig->setMessage(UbxGpsConfigMessage::NavPvt);
    ubxGpsConfig->setRate(200);
    ubxGpsConfig->configure();
    delete ubxGpsConfig;

    ubx_gps->begin(GPS_BAUDRATE, SERIAL_8N1, TX_PIN, RX_PIN);

    Serial.println("GPS interface initialized");
  }

  static GpsReader &getInstance()
  {
    static GpsReader instance;
    return instance;
  }
};