
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include "../config.h"
#include "LiLyGo_TGO.hpp"

#include "../../fonts/NotoSansBold15.h"
#include "../../fonts/Inconsolata_130_numbers_only.h"


#define AA_FONT_SMALL NotoSansBold15
#define AA_FONT_LARGE IncoNums

//#define _DEBUG

  TFT_eSPI tft = TFT_eSPI(135, 240); // Invoke custom library

  void TGO::init()
  {
    /*Turn on power control*/
    pinMode(ADC_EN, OUTPUT);
    digitalWrite(ADC_EN, HIGH);

    tft.init();
    tft.setRotation(0);
    tft.setSwapBytes(true);
  
    tft.fillScreen(TFT_GREEN);

    Serial.println("T-GO initiated");
  }




  void TGO::displayTop() {

    //float VBAT = (float)(analogRead(ADC_EN)) * 3600 / 4095 * 2 / 1000;
  /*
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
    */
  }


  void TGO::displaySide() {
    /*
    VBAT = (float)(analogRead(vbatPin)) * 3600 / 4095 * 2 / 1000;
    u8g2.setFont(u8g2_font_profont11_mf  );  
    u8g2.setFontDirection(3);
    u8g2.setCursor(125, 55);
    u8g2.print(VBAT,1);
    u8g2.print("v ");
    u8g2.print(gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites());
    u8g2.println("¤");  
    */
  }



  void TGO::displayDigits(int current, int previous){

    tft.loadFont(AA_FONT_LARGE);
    tft.setRotation(1);
    tft.setTextDatum(TL_DATUM);

    if((previous != current)){
      if (previous !=0){
        tft.setTextColor(TFT_BLACK, TFT_BLACK);
        tft.drawString(String(previous), 5 , 15 );
      } else {
        tft.fillScreen(TFT_BLACK);
      }
      tft.setTextColor(TFT_GREEN, TFT_BLACK);    
      tft.drawString(String(current), 5 , 15 );
    }
  }

void TGO::clearScreen(){
  tft.fillScreen(TFT_BLACK);
}

void TGO::displayString(char* string, int x, int y){  
  tft.setRotation(1);
  tft.setTextDatum(TL_DATUM);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.loadFont(AA_FONT_SMALL); 

  tft.drawString(string, x , y);
}