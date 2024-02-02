import math

class DataPoint:
    A_GRAVITY = 9.81  # Earth's gravitational acceleration

    def __init__(self):
        self.ts = 0
        self.hasGeodetic = False
        self.isValid = False

        self.lat = 0.0
        self.lon = 0.0
        self.hMSL = 0.0

        self.velN = 0.0
        self.velE = 0.0
        self.velD = 0.0

        self.hAcc = 0.0
        self.vAcc = 0.0
        self.sAcc = 0.0

        self.heading = 0.0
        self.cAcc = 0.0

        self.gpsFix = 0
        self.numSV = 0

        self.t = 0.0
        self.x = 0.0
        self.y = 0.0
        self.z = 0.0

        self.dist2D = 0.0
        self.dist3D = 0.0

        self.curv = 0.0
        self.accel = 0.0

        self.ax = 0.0
        self.ay = 0.0
        self.az = 0.0
        self.amag = 0.0

        self.lift = 0.0
        self.drag = 0.0

        self.vx = 0.0
        self.vy = 0.0

        self.theta = 0.0
        self.omega = 0.0

    @staticmethod
    def interpolate(p1, p2, a):
        result = DataPoint()
        result.vx = p1.vx + a * (p2.vx - p1.vx)
        result.vy = p1.vy + a * (p2.vy - p1.vy)
        # Continue interpolating other fields as needed
        return result

    @staticmethod
    def elevation(dp):
        return dp.z

    @staticmethod
    def northSpeed(dp):
        return dp.vy

    @staticmethod
    def eastSpeed(dp):
        return dp.vx

    @staticmethod
    def northSpeedRaw(dp):
        return dp.velN

    @staticmethod
    def eastSpeedRaw(dp):
        return dp.velE

    @staticmethod
    def verticalSpeed(dp):
        return dp.velD

    @staticmethod
    def horizontalSpeed(dp):
        return math.sqrt(dp.velE * dp.velE + dp.velN * dp.velN)

    @staticmethod
    def totalSpeed(dp):
        return math.sqrt(dp.velE * dp.velE + dp.velN * dp.velN + dp.velD * dp.velD)

    @staticmethod
    def diveAngle(dp):
        pi = 3.14159265359
        return math.atan2(dp.velD, math.sqrt(dp.vx * dp.vx + dp.vy * dp.vy)) / pi * 180

    @staticmethod
    def curvature(dp):
        return dp.curv

    @staticmethod
    def glideRatio(dp):
        try:
            if dp.velD != 0:
                return math.sqrt(dp.velE * dp.velE + dp.velN * dp.velN) / dp.velD
            else:
                return 0
        except AttributeError as e:
            return 0

    @staticmethod
    def horizontalAccuracy(dp):
        return dp.hAcc

    @staticmethod
    def verticalAccuracy(dp):
        return dp.vAcc

    @staticmethod
    def speedAccuracy(dp):
        return dp.sAcc

    @staticmethod
    def numberOfSatellites(dp):
        return dp.numSV

    @staticmethod
    def time(dp):
        return dp.t

    @staticmethod
    def distance2D(dp):
        return dp.dist2D

    @staticmethod
    def distance3D(dp):
        return dp.dist3D

    @staticmethod
    def acceleration(dp):
        return dp.accel

    @staticmethod
    def accForward(dp):
        return dp.ax

    @staticmethod
    def accRight(dp):
        return dp.ay

    @staticmethod
    def accDown(dp):
        return dp.az

    @staticmethod
    def accMagnitude(dp):
        return dp.amag

    @staticmethod
    def totalEnergy(dp):
        v = DataPoint.totalSpeed(dp)
        return v * v / 2 + DataPoint.A_GRAVITY * DataPoint.elevation(dp)

    @staticmethod
    def energyRate(dp):
        return DataPoint.totalSpeed(dp) * DataPoint.acceleration(dp) - DataPoint.A_GRAVITY * DataPoint.verticalSpeed(dp)

    @staticmethod
    def liftCoefficient(dp):
        return dp.lift

    @staticmethod
    def dragCoefficient(dp):
        return dp.drag

    @staticmethod
    def course(dp):
        return dp.theta

    @staticmethod
    def courseRate(dp):
        return dp.omega

    @staticmethod
    def courseAccuracy(dp):
        return dp.cAcc