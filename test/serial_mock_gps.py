import serial
import time

path = "E:\\Dev\\mockGPS\\data4_1.csv"

with open (path, "r") as f:
    with serial.Serial('COM9', 115200, timeout=1) as ser:
        line = f.readline()
        while line:
            req = line.strip() + ",\n"
            ser.write(req.encode())
            resp = ser.readline()
            if len(resp) > 0:
                print(resp.decode())
            line = f.readline()
            time.sleep(0.2)