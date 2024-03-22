#pragma once
#include "../common.h"
#include "GpsInterface.hpp"
#include "GpsMock.cpp"
#include "GpsReader.cpp"

#define SLOPE_LEN 4

class DataProvider
{
private:
    DataPoint timeseries[SLOPE_LEN];
    DataPoint curDp;
    DataPoint exitDp;
    GpsInterface& gps;

    long long exitTs = 0;
    long long firstSignalTs = 0;
    bool alreadyFalling = false;

    // DataProvider(){}

    DataProvider(GpsInterface& gps) : gps(gps) {}


    void addElement(DataPoint value, DataPoint array[], int size)
    {
        for (int i = 0; i < size - 1; ++i)
        {
            array[i] = array[i + 1];
        }
        array[size - 1] = value;
    }

    void printDouble( double val, unsigned int precision){

        Serial.print (int(val));  //prints the int part
        Serial.print("."); // print the decimal point
        unsigned int frac;
        if(val >= 0)
            frac = (val - int(val)) * precision;
        else
            frac = (int(val)- val ) * precision;
        Serial.print(frac,DEC) ;
    } 
 

    double getSlope()
    {
        float sumx = 0, sumy = 0, sumxx = 0, sumxy = 0;

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
        // Serial.print("sumx: "); Serial.print(sumx); Serial.print(" sumy: "); Serial.print(sumy);Serial.print(" sumxx: "); Serial.print(sumxx);Serial.print(" sumxy: "); Serial.print(sumxy);
        // Serial.print("Slope: ");printDouble(slope,6);
        return slope;
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
        if (a < 0 || 1 < a){
            // Serial.println();
            return false;
        }

        // Check accuracy
        double vAcc = timeseries[p].vAcc + a * (timeseries[c].vAcc - timeseries[p].vAcc);
        if (vAcc > 10){
            // Serial.println();
            return false;
        }
        // Check acceleration
        double az = timeseries[p].az + a * (timeseries[c].az - timeseries[p].az);
        if (az < g / 5.){
            // Serial.println();
            return false;
        }

        int t_diff = timeseries[c].ts - timeseries[p].ts;
        exitTs = (long long)(timeseries[p].ts + a * t_diff - g / az * 1000.);
        // Serial.print("Exit! ");Serial.print(exitTs);
        // Serial.print(" p.ts: ");Serial.print(timeseries[p].ts); Serial.print(" c.ts: ");Serial.print(timeseries[c].ts);Serial.print(" a: ");Serial.print(a);Serial.print(" az: ");Serial.print(az);
        // Serial.print(" p.az: ");Serial.print(timeseries[p].az); Serial.print(" c.az: ");Serial.println(timeseries[c].az);

        exitDp = curDp;
        alreadyFalling = true;
        return alreadyFalling;
    }

    // DataPoint getStartDp()
    // {
    //   return runStartDp;
    // }

public:

    static DataProvider& getInstance()
    {
        static DataProvider instance(GpsReader::getInstance());
        return instance;
    }

    static void init(HardwareSerial &port)
    {
        GpsReader::getInstance().init(port);
    }

    void refresh()
    {

        gps.refresh();

        curDp = gps.getLatestData();

        if (firstSignalTs != 0)
        {
            curDp.t = (double)(curDp.ts - firstSignalTs) / 1000;
            addElement(curDp, timeseries, SLOPE_LEN);
        }
        else
        {
            firstSignalTs = gps.getLatestData().ts;
        }

        if (!alreadyFalling)
        {
            detectFreefall();
        }
    }

    long long getExitTs()
    {
        return exitTs;
    }

    DataPoint getExitDp()
    {
        return exitDp;
    }

    DataPoint getDataPoint()
    {
        return curDp;
    }
};