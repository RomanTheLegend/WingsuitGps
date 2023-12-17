#pragma once
#include "../common.h"
#include "GpsInterface.hpp"
#include "GpsMock.cpp"

#define SLOPE_LEN 4

class DataProvider
{
private:
    DataPoint timeseries[SLOPE_LEN];
    DataPoint curDp;
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

public:

    static DataProvider& getInstance()
    {
        static DataProvider instance(GpsMock::getInstance());
        return instance;
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

    long getExitTs()
    {
        return exitTs;
    }

    DataPoint getDataPoint()
    {
        return curDp;
    }
};