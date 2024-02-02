from DataPoint import DataPoint

SLOPE_LEN = 4
A_GRAVITY = 9.81  # Earth's gravitational acceleration

def get_slope(timeseries):
    sumx, sumy, sumxx, sumxy = 0, 0, 0, 0

    for i in range(SLOPE_LEN):
        y = timeseries[i].velD
        t = timeseries[i].t

        sumx += t
        sumy += y
        sumxx += t * t
        sumxy += t * y

    n = SLOPE_LEN
    slope = (sumxy - sumx * sumy / n) / (sumxx - sumx * sumx / n)
    return slope

def detect_freefall(timeseries):
    timeseries[SLOPE_LEN - 1].az = get_slope(timeseries)
    g = A_GRAVITY

    p, c = SLOPE_LEN - 2, SLOPE_LEN - 1

    a = (g - timeseries[p].velD) / (timeseries[c].velD - timeseries[p].velD)
    if a < 0 or a > 1:
        return None

    v_acc = timeseries[p].vAcc + a * (timeseries[c].vAcc - timeseries[p].vAcc)
    if v_acc > 10:
        return None

    az = timeseries[p].az + a * (timeseries[c].az - timeseries[p].az)
    if az < g / 5.0:
        return None

    exit_ts = int(timeseries[p].ts + a * (timeseries[c].ts - timeseries[p].ts) - g / az * 1000)

    exit_dp = DataPoint()
    exit_dp.ts = exit_ts
    exit_dp.lat = timeseries[p].lat + a * (timeseries[c].lat - timeseries[p].lat)
    exit_dp.lon = timeseries[p].lon + a * (timeseries[c].lon - timeseries[p].lon)

    # Assuming exitDp and curDp are defined globally
    # exit_dp = curDp

    return exit_dp
