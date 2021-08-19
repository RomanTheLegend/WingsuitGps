
#include <Arduino.h>
#include <HardwareSerial.h>
#include <TinyGPS.h>
#include <SPI.h>
#include <Wire.h>
#include <U8g2lib.h>
#include "utilities.h"

//#define _DEBUG


U8G2_SSD1309_128X64_NONAME2_F_4W_SW_SPI u8g2(U8G2_R0,
        /* clock=*/ OLED_SCLK,
        /* data=*/  OLED_MOSI,
        /* cs=*/    OLED_CS,
        /* dc=*/    OLED_DC,
        /* reset=*/ OLED_RST);

TinyGPS gps;
HardwareSerial SerialGPS ( 1 );



void setup()
{
  Serial.begin(115200);
  Serial.println("Starting up");
  SerialGPS.begin ( 9600 , SERIAL_8N1, 21 , 22 );


  /*Turn on power control*/
  pinMode(PWR_ON, OUTPUT);
  digitalWrite(PWR_ON, HIGH);

  /*Power on the display*/
  pinMode(OLED_PWR, OUTPUT);
  digitalWrite(OLED_PWR, HIGH);

  /*Set touch Pin as input*/
  pinMode(TOUCH_PIN, INPUT);

  u8g2.begin();

  u8g2.enableUTF8Print(); // enable UTF8 support for the Arduino print() function


  Serial.println("initialization done.");
  Serial.println("Ready");

  displaySpeed(0);
}

void loop()
{
  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;
  float flat;
  float flon;
  float altit;
  unsigned long age;
  int YY;
  byte MM;
  byte DD;
  byte hrs;
  byte mins;
  byte secs;
  byte hunds;

  // For one second we parse GPS data and report some key values
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (SerialGPS.available())
    {
      char c = SerialGPS.read();
      Serial.write(c); // uncomment this line if you want to see the GPS data flowing
      if (gps.encode(c)) // Did a new valid sentence come in?
        newData = true;        
    }
  }


  if (newData)
  {
    gps.f_get_position(&flat, &flon, &age);
    gps.crack_datetime(&YY, &MM, &DD, &hrs, &mins, &secs, &hunds, &age);
    altit = gps.altitude();



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
    displaySpeed(gps.f_speed_kmph());
    delay(500);
//    displayCourse(gps.f_course());
  }

//  Serial.print(" SAT=");
//  Serial.println(gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites());
  
  gps.stats(&chars, &sentences, &failed);
  if (chars == 0) {
    Serial.println("** No characters received from GPS: check wiring **");    
  }
}

void displaySpeed(float spd) {
  //spd = 172.1;
  int ispd = int(spd);
  
  //byte satelites = byte( (gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites()) %10);

  u8g2.setFont(u8g2_font_inb46_mn);  
  u8g2.setFontDirection(0);
  u8g2.clearBuffer();
  u8g2.setCursor(0, 50);
  u8g2.print(ispd);
  //u8g2.setFont(u8g2_font_unifont_t_chinese2); 
  //u8g2.setCursor(95, 40);
  //u8g2.print("km/h");
  u8g2.sendBuffer();

}
/*
void displayCourse(float course) {
  //course = 270.8;
  int icourse = int(course);
  
  // Split the degrees into digits
  byte digit2 = byte((icourse / 100) % 10);
  byte digit1 = byte((icourse / 10) % 10);
  byte digit0 = byte((icourse % 10));
  tm.clearDisplay();
  // Display the data and also implement Leading-Zero Blanking (LZB)

  tm.display(0,'_');

  tm.display(3,digit0);
  if(course > 9.99)
    tm.display(2,digit1);
  if(course > 99.99)
    tm.display(1,digit2);
}
*/

