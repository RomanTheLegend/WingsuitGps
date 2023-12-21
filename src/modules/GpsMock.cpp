#pragma once
#include <Arduino.h>
#include <HardwareSerial.h>
#include "GpsInterface.hpp"
#include "../common.h"
#include <TimeLib.h>

// #define MOCK_DEBUG

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

        long long t = makeTime(tm);
        t = t * 1000 + milis;

        return t;
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
#ifdef MOCK_DEBUG
            Serial.println("received data:");
            Serial.println(line);
            String line_orig = line;
#endif

            int pos = 0;
            int field = 0;
            String token;

            //ToDo: fix bug with parsing last field - i.e. when there's no last comma
            while ((pos = line.indexOf(',')) != -1)
            {
                field++;
                token = line.substring(0, pos);
                // Serial.print("#");Serial.println(field);
                // Serial.print(" pos=");Serial.println(pos);
                // Serial.print(" token=");Serial.println(token);
                // Serial.print(" line=");Serial.println(line);

                line.remove(0, pos + 1);
                // Serial.print(" line2=");Serial.println(line);
                // Serial.println(" ---");

                switch (field)
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

#ifdef MOCK_DEBUG
            Serial.println(line_orig);
            Serial.print("ts=");Serial.print(_curDp.ts);
            Serial.print(" lat=");Serial.print(_curDp.lat);
            Serial.print(" lon=");Serial.print(_curDp.lon);
            Serial.print(" hMSL=");Serial.print(_curDp.hMSL);
            Serial.print(" velN=");Serial.print(_curDp.velN);
            Serial.print(" velE=");Serial.print(_curDp.velE);
            Serial.print(" velD=");Serial.print(_curDp.velD);
            Serial.print(" hAcc=");Serial.print(_curDp.hAcc);
            Serial.print(" vAcc=");Serial.print(_curDp.vAcc);
            Serial.print(" sAcc=");Serial.print(_curDp.sAcc);
            Serial.print(" heading=");Serial.print(_curDp.heading);
            Serial.print(" cAcc=");Serial.print(_curDp.cAcc);
            Serial.print(" numSV=");Serial.println(_curDp.numSV);
#endif
            curDp = _curDp;
        }
    }

    DataPoint getLatestData()
    {
        return curDp;
    }
};
