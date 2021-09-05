
#include <Arduino.h>
#include <HardwareSerial.h>
#include <TinyGPS.h>
#include <SPI.h>
#include <Wire.h>
#include <U8g2lib.h>
#include "utilities.h"

//#define _DEBUG
const uint8_t vbatPin = 34;

U8G2_SSD1309_128X64_NONAME2_F_4W_SW_SPI u8g2(U8G2_R0,
        /* clock=*/ OLED_SCLK,
        /* data=*/  OLED_MOSI,
        /* cs=*/    OLED_CS,
        /* dc=*/    OLED_DC,
        /* reset=*/ OLED_RST);

TinyGPS gps;
HardwareSerial SerialGPS ( 1 );

float VBAT= 0; // battery voltage from ESP32 ADC read

volatile int random_speed = 140;

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

  pinMode(vbatPin, INPUT);

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
  for (unsigned long start = millis(); millis() - start < 250;)
  {
    while (SerialGPS.available())
    {
      char c = SerialGPS.read();
      //Serial.write(c); // uncomment this line if you want to see the GPS data flowing
      if (gps.encode(c)) // Did a new valid sentence come in?
        newData = true;        
    }
  }

  if (newData)
  {
    gps.f_get_position(&flat, &flon, &age);
    gps.crack_datetime(&YY, &MM, &DD, &hrs, &mins, &secs, &hunds, &age);
    //altit = gps.altitude();



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

//    displayCourse(gps.f_course());
  } else {
    //displayStats();
    //displayRandomSpeed();
  }

  gps.stats(&chars, &sentences, &failed);
  if (chars == 0) {
    Serial.println("** No characters received from GPS: check wiring **");    
  }
}


void displayStats() {

  VBAT = (float)(analogRead(vbatPin)) * 3600 / 4095 * 2 / 1000;

  u8g2.setFont(u8g2_font_t0_11_mf   );  
  u8g2.setFontDirection(0);
  u8g2.clearBuffer();
  u8g2.setCursor(0, 10);
  u8g2.println("Vbat = ");
  u8g2.print(VBAT,2);
  u8g2.println(" Volts");

  u8g2.setCursor(0, 40);
  u8g2.print(" SAT=");
  u8g2.println(gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites());

  u8g2.sendBuffer();
  delay(250);
}


void displayMiniStats() {
  VBAT = (float)(analogRead(vbatPin)) * 3600 / 4095 * 2 / 1000;
  u8g2.setFont(u8g2_font_profont11_mf  );  
  u8g2.setFontDirection(3);
  u8g2.setCursor(125, 55);
  u8g2.print(VBAT,1);
  u8g2.print("v ");
  u8g2.print(gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites());
  u8g2.println("¤");  
}


void displayRandomSpeed(){
    int randNumber = random(3);
    if (random_speed < randNumber){
        random_speed = random_speed + randNumber;
    } else {
      int direction =  random(4);
      if (direction > 1){
        random_speed = random_speed + randNumber;        
      } else {
        random_speed = random_speed - randNumber;
      }
    }
    
    displaySpeed(random_speed);
}


void displaySpeed(float spd) {
  //spd = 172.1;
  int ispd = int(spd);

  u8g2.setFont(u8g2_font_inb46_mn);  
  u8g2.setFontDirection(0);
  u8g2.clearBuffer();
  u8g2.setCursor(0, 50);
  u8g2.print(ispd);
  //u8g2.setFont(u8g2_font_unifont_t_chinese2); 
  //u8g2.setCursor(95, 40);
  //u8g2.print("km/h");
  displayMiniStats();
  u8g2.sendBuffer();
  delay(250);
}


void displayCourse(float course) {
  //course = 270.8;
  int icourse = int(course);
  u8g2.setFont(u8g2_font_inb46_mn);  
  u8g2.setFontDirection(0);
  u8g2.clearBuffer();
  u8g2.setCursor(0, 50);
  u8g2.print(icourse);
  u8g2.sendBuffer();
  delay(250);
}

