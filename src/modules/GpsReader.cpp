// #include <Arduino.h>
// #include <HardwareSerial.h>
// #include <SPI.h>
// #include <Wire.h>
// #include "GpsInterface.hpp"
// #include "BluetoothInterface.hpp"
// #include "../common.h"
// #include "config.h"

// // #define GPS_DEBUG
// #define GPS_BAUDRATE 115200

// #define DATETIME_FORMAT "%04d.%02d.%02d %02d:%02d:%02d"
// #define DATETIME_LENGTH 20


// class GpsReader : public GpsInterface
// {
// private:

//   DataPoint curDp;
//   DataPoint _curDp;
//   HardwareSerial SerialGPS(2);
//   UbxGpsNavPvt<HardwareSerial> gps(SerialGPS);


//   char datetime[DATETIME_LENGTH];

//   GpsReader()
//   {

//     UbxGpsConfig<HardwareSerial, HardwareSerial> *ubxGpsConfig = new UbxGpsConfig<HardwareSerial, HardwareSerial>(SerialGPS, Serial);
//     ubxGpsConfig->setCustomPins(TX_PIN, RX_PIN);
//     ubxGpsConfig->setBaudrate(GPS_BAUDRATE);
//     ubxGpsConfig->setMessage(UbxGpsConfigMessage::NavPvt);
//     ubxGpsConfig->setRate(200);
//     ubxGpsConfig->configure();
//     delete ubxGpsConfig;

//     gps.begin(GPS_BAUDRATE, SERIAL_8N1, TX_PIN, RX_PIN);

//     Serial.println("GPS interface initialized");
//   }



// public:

//   void refresh()
//   {

//     if (gps.ready())
//     {
// #ifdef GPS_DEBUG
//       snprintf(datetime, DATETIME_LENGTH, DATETIME_FORMAT, gps.year, gps.month, gps.day, gps.hour, gps.min, gps.sec);

//       Serial.print(datetime);
//       Serial.print(", Lon: ");
//       Serial.print(gps.lon / 10000000.0, 7);
//       Serial.print(" , Lat: ");
//       Serial.print(gps.lat / 10000000.0, 7);
//       Serial.print(" , Height: ");
//       Serial.print(gps.height / 1000.0, 3);
//       Serial.print(" , Speed: ");
//       Serial.print(gps.gSpeed * 0.0036, 5);
//       Serial.print(" , Heading: ");
//       Serial.print(gps.heading / 100000.0, 5);
//       Serial.print(" , Fix type: ");
//       Serial.print(gps.fixType);
//       Serial.print(" , Sats: ");
//       Serial.println(gps.numSV);
// #endif
//       curDp = _curDp;
//     }
//     // while (SerialGPS.available())
//     // {
//     //   char c = SerialGPS.read();
//     //   BluetoothInterface::sendData(c);
//     //   //Serial.write(c); // uncomment this line if you want to see the GPS data flowing
//     //   if (gps.encode(c)) // Did a new valid sentence come in?
//     //     newData = true;
//     // }
//   }

//   DataPoint getLatestData()
//   {
//     return curDp;
//   }

//   static GpsReader& getInstance() {
//       static GpsReader instance;
//       return instance;
//   }
// };