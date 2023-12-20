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

    long long convertToUnixEpoch(const String &rawString)
    {
        char timeString[24];
        rawString.toCharArray(timeString, 24);
        TimeElements tm;
        int year, month, day, hour, minute, second;

        // Parse the timestamp string
        // ex. 2023-08-24T12:59:14.400Z

        tm.Year = CalendarYrToTm((timeString[0] - '0') * 1000 + (timeString[1] - '0') * 100 + (timeString[2] - '0') * 10 + (timeString[3] - '0'));
        tm.Month = (timeString[5] - '0') * 10 + (timeString[6] - '0');
        tm.Day = (timeString[8] - '0') * 10 + (timeString[9] - '0');
        tm.Hour = (timeString[11] - '0') * 10 + (timeString[12] - '0');
        tm.Minute = (timeString[14] - '0') * 10 + (timeString[15] - '0');
        tm.Second = (timeString[17] - '0') * 10 + (timeString[18] - '0');
        int milis = (timeString[20] - '0') * 100 + (timeString[21] - '0') * 10 + (timeString[22] - '0');

        // Assemble time elements into time_t.
        time_t t = makeTime(tm);
        return long(t)*1000 + milis;
    }

public:
    static GpsMock &getInstance()
    {
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

                switch (pos)
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
