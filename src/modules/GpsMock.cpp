#pragma once
#include <Arduino.h>
#include "GpsInterface.hpp"
#include "../common.h"
#include <TimeLib.h>

class GpsMock : public GpsInterface
{
private:
  DataPoint curDp;
  DataPoint _curDp;

  unsigned long long convertToUnixEpoch(const String &timestamp)
  {
    tmElements_t tm;
    int year, month, day, hour, minute, second;

    // Parse the timestamp string
    sscanf(timestamp.c_str(), "%d-%d-%dT%d:%d:%d", &year, &month, &day, &hour, &minute, &second);

    tm.Year = CalendarYrToTm(year);
    tm.Month = month;
    tm.Day = day;
    tm.Hour = hour;
    tm.Minute = minute;
    tm.Second = second;

    // Check if there is a fraction of seconds
    if (timestamp.indexOf('.') != -1)
    {
      int dotIndex = timestamp.indexOf('.');
      int ZIndex = timestamp.indexOf('Z');

      // Read the fraction of seconds
      String fractionString = timestamp.substring(dotIndex + 1, ZIndex);

      // Convert fractionString to an integer if it is not empty
      int fractionSeconds = (!fractionString.isEmpty()) ? fractionString.toInt() : 0;

      time_t epochTime = makeTime(tm);
      unsigned long long milliseconds = epochTime * 1000;

      // Add the fraction of seconds in milliseconds
      return milliseconds + fractionSeconds * 10 + 3600000;
    }
    else
    {
      time_t epochTime = makeTime(tm);
      return epochTime * 1000;
    }
  }

public:

  static GpsMock& getInstance() {
      static GpsMock instance;
      return instance;
  }

  void refresh()
  {
    String line;

    if (Serial.available() > 0)
    {
      line = Serial.readStringUntil('\n');

      int pos = 0;
      String token;
      while ((pos = line.indexOf(',')) != -1)
      {
        token = line.substring(0, pos);
        line.remove(0, pos + 1);

        switch (_curDp.ts)
        {
          case 1:
            _curDp.ts = convertToUnixEpoch(token);
            break;

          case 2:
            _curDp.lat = token.toFloat();
            break;

          case 3:
            _curDp.lon = token.toFloat();
            break;

          case 4:
            _curDp.hMSL = token.toFloat();
            break;

          case 5:
            _curDp.velN = token.toFloat();
            break;

          case 6:
            _curDp.velE = token.toFloat();
            break;

          case 7:
            _curDp.velD = token.toFloat();
            break;

          case 8:
            _curDp.hAcc = token.toFloat();
            break;

          case 9:
            _curDp.vAcc = token.toFloat();
            break;

          case 10:
            _curDp.sAcc = token.toFloat();
            break;

          case 11:
            _curDp.heading = token.toFloat();
            break;

          case 12:
            _curDp.cAcc = token.toFloat();
            break;

          case 14:
            _curDp.numSV = token.toFloat();
            break;
        }

      }

      curDp = _curDp;
    }
  }

  DataPoint getLatestData()
  {
    return curDp;
  }
};
