#include <Arduino.h>
#include <HardwareSerial.h>
#include <TinyGPS.h>
#include <SPI.h>
#include <Wire.h>
#include "GpsInterface.hpp"
#include "BluetoothInterface.hpp"

TinyGPS gps;

HardwareSerial SerialGPS ( 1 );


namespace GpsInterface { 


  void sendPacket(byte *packet, byte len);
  void changeBaudrate();
  void changeFrequency();

  void init(int tx, int rx){

    // Default baud rate is 9600 which is 2 sentences per second (1 sentence = 4800 bytes)
    // Device is locked at that rate at the factory, so if you want higher baut rate i.e. 38 400 (like in FlySight)
    // then you need some tools and a bit of patching: https://www.waveshare.com/wiki/UART_GPS_NEO-6M
    // ToDo: switch to UBX protocol which is faster tha NMEA: https://www.arduino.cc/reference/en/libraries/ubxgps/

    //https://cglabs.medium.com/neo-6m-howto-change-serial-data-rate-9-6k-to-115k-for-neo-6-neo-7-neo-8-66d9898c1445
  

    SerialGPS.begin ( 9600 , SERIAL_8N1, tx , rx );
    
    changeFrequency();
    delay(200);

    SerialGPS.flush();

    changeBaudrate();
    
    delay(200);

    SerialGPS.flush();
   /*
    SerialGPS.end();
    delay(300);
    */
    SerialGPS.begin ( 34800 , SERIAL_8N1, tx , rx );
   
    Serial.println("GPS interface initialized");

  }


  void loop(){

    bool newData = false;
    unsigned long chars;
    unsigned short sentences, failed;
    float flat;
    float flon;
    //float altit;
    unsigned long age;
    int YY;
    byte MM;
    byte DD;
    byte hrs;
    byte mins;
    byte secs;
    byte hunds;

    char dateString[32];

    // For 200 milliseconds we parse GPS data and report some key values
    for (unsigned long start = millis(); millis() - start < 200;)
    {
      while (SerialGPS.available())
      {
        char c = SerialGPS.read();
        BluetoothInterface::sendData(c);
        //Serial.write(c); // uncomment this line if you want to see the GPS data flowing
        if (gps.encode(c)) // Did a new valid sentence come in?
          newData = true;
      }
    }

    if (newData)
    {
      gps.f_get_position(&flat, &flon, &age);
      gps.crack_datetime(&YY, &MM, &DD, &hrs, &mins, &secs, &hunds, &age);

      sprintf(dateString, "%02d/%02d/%02d %02d:%02d:%02d.%03u",
        MM, DD, YY, hrs, mins, secs, hunds);  
      //Serial.print("Lat="); Serial.print(flat,6);  Serial.print(" Lon=");  Serial.print(flon,6); Serial.print(" Date="); Serial.print(dateString); Serial.print(" Sat="); Serial.print(gps.satellites()); Serial.print(" hdop="); Serial.println(gps.hdop());

  #ifdef _DEBUG    
      Serial.print("LAT=");
      Serial.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
      Serial.print(" LON=");
      Serial.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
      Serial.print(" SAT=");
      Serial.print(gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites());
      Serial.print(" PREC=");
      Serial.print(gps.hdop() == TinyGPS::GPS_INVALID_HDOP ? 0 : gps.hdop());
      Serial.println();
  #endif

    } 

    gps.stats(&chars, &sentences, &failed);

    if (chars == 0) {
      //Serial.println("** No characters received from GPS: check wiring **");    
    } else {
      //Serial.print(gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites()); Serial.println(" satelites in sight");
    }
  }

  TinyGPS* getGps()
  {
      return &gps;
  }

// Send a packet to the receiver to change baudrate to 38 400
void changeBaudrate() {
    byte packet38400[] = {
      0xB5, 0x62, 0x06, 0x00, 0x14, 0x00, 0x01, 0x00, 0x00, 
      0x00, 0xD0, 0x08, 0x00, 0x00, 0xF0, 0x87, 0x00, 0x00, 
      0x07, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x74, 0x24,
    };

    byte packet115200[] = {
      0xB5, 0x62, 0x06, 0x00, 0x14, 0x00, 0x01, 0x00, 0x00, 
      0x00, 0xD0, 0x08, 0x00, 0x00, 0x00, 0xC2, 0x01, 0x00, 
      0x07, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x7E,
    };
    byte *packet = packet38400;
    sendPacket(packet, sizeof(packet));
}

void changeFrequency() {
    byte packet[] = {
      0xB5,0x62,0x06,0x08,0x06,0x00,0xC8,0x00,0x01,0x00,0x01,0x00,0xDE,0x6A,
    };
    sendPacket(packet, sizeof(packet));
}

void sendPacket(byte *packet, byte len) {
    for (byte i = 0; i < len; i++)
    {
        SerialGPS.write(packet[i]);
    }
}

}