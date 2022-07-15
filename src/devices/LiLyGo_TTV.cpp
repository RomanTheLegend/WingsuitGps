#include "LiLyGo_TTV.hpp"
#ifdef USE_TTV
#include "LiLyGo_TTV_pin_setup.h"

//#define _DEBUG


U8G2_SSD1309_128X64_NONAME2_F_4W_SW_SPI u8g2(U8G2_R0,
        /* clock=*/ OLED_SCLK,
        /* data=*/  OLED_MOSI,
        /* cs=*/    OLED_CS,
        /* dc=*/    OLED_DC,
        /* reset=*/ OLED_RST);



void TTV::init()
{

  pinMode(PWR_ON, OUTPUT);
  digitalWrite(PWR_ON, HIGH);

  pinMode(OLED_PWR, OUTPUT);
  digitalWrite(OLED_PWR, HIGH);

  pinMode(ADC_PIN, INPUT);

  u8g2.begin();
  u8g2.enableUTF8Print(); // enable UTF8 support for the Arduino print() function
 
  Serial.println("TTV initialization done.");
}



void displayStats() {

  float VBAT = (float)(analogRead(ADC_PIN)) * 3600 / 4095 * 2 / 1000;

  u8g2.setFont(u8g2_font_t0_11_mf   );  
  u8g2.setFontDirection(0);
  u8g2.clearBuffer();
  u8g2.setCursor(0, 10);
  u8g2.println("Vbat = ");
  u8g2.print(VBAT,2);
  u8g2.println(" Volts");

  u8g2.setCursor(0, 40);
  u8g2.print(" SAT=");
 // u8g2.println(gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites());

  u8g2.sendBuffer();
  delay(250);
}


void displayMiniStats() {
  float VBAT = (float)(analogRead(ADC_PIN)) * 3600 / 4095 * 2 / 1000;
  u8g2.setFont(u8g2_font_profont11_mf  );  
  u8g2.setFontDirection(3);
  u8g2.setCursor(125, 55);
  u8g2.print(VBAT,1);
  u8g2.print("v ");
 // u8g2.print(gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites());
  u8g2.println("¤");  
}


void TTV::displayDigits(int current, int previous) {

  u8g2.setFont(u8g2_font_inb46_mn);  
  u8g2.setFontDirection(0);
  u8g2.clearBuffer();
  u8g2.setCursor(0, 50);
  u8g2.print(current);
  //u8g2.setFont(u8g2_font_unifont_t_chinese2); 
  //u8g2.setCursor(95, 40);
  //u8g2.print("km/h");
  displayMiniStats();
  u8g2.sendBuffer();
  //delay(250);
}

void TTV::clearScreen(){
  u8g2.clearBuffer();
}


void TTV::displayString(char* string, int x, int y){

  u8g2.setFont(u8g2_font_t0_11_mf);  
  u8g2.setFontDirection(0);  
  
  u8g2.setCursor(x, y);
  u8g2.println(string);

  u8g2.sendBuffer();
  //delay(250);
}

void TTV::displaySide(){

}

void TTV::displayTop(){
  
}
#endif