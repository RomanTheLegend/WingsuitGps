#include <Arduino.h>
#include <HardwareSerial.h>
#include <SPI.h>
#include <Wire.h>
#include "GpsInterface.hpp"
#include "BluetoothInterface.hpp"


// #define GPS_DEBUG

namespace GpsInterface { 

  HardwareSerial SerialGPS(2);

  UbxGpsNavPvt<HardwareSerial> gps(SerialGPS);

  #define GPS_BAUDRATE 115200

  #define DATETIME_FORMAT "%04d.%02d.%02d %02d:%02d:%02d"
  #define DATETIME_LENGTH 20
  char datetime[DATETIME_LENGTH];


  void init(int tx, int rx){


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


  void loop(){

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

  template <class HardwareSerial>
  UbxGpsNavPvt<HardwareSerial> getGps()
  {
      return gps;
  }

  long getSpeed(){
    return long (gps.gSpeed * 0.0036);
  }

  long getHeading(){
    return long(gps.heading / 100000.0 + 0.5);
  }
}