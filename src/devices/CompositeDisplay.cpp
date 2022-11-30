
#include <ESP_8_BIT_GFX.h>
#include "../config.h"
#include "CompositeDisplay.hpp"
/*
#include "../../fonts/NotoSansBold15.h"
#include "../../fonts/Inconsolata_130_numbers_only.h"


#define AA_FONT_SMALL NotoSansBold15
#define AA_FONT_LARGE IncoNums
*/
#define _DEBUG

ESP_8_BIT_GFX videoOut(true /* = NTSC */, 8 /* = RGB332 color */);


void CompositeDisplay::init()
{
  videoOut.begin();
  videoOut.setRotation(3);
  videoOut.setTextSize(2);
  videoOut.copyAfterSwap = true;
  Serial.println("Composite Video initiated");
}




  void CompositeDisplay::displayTop() {

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


void CompositeDisplay::displaySide() {
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



void CompositeDisplay::displayDigits(int current, int previous){
  if((previous != current)){
    if (previous !=0){
      videoOut.setTextColor(TFT_BLACK, TFT_BLACK);
      videoOut.setCursor(5, 15);
      videoOut.print(String(previous));
    } else {
      videoOut.fillScreen(0);
    }
    videoOut.setTextColor(TFT_GREEN, TFT_BLACK);

    videoOut.setCursor(5, 15);
    videoOut.print(String(current));
  }

}

void CompositeDisplay::clearScreen(){
  videoOut.fillScreen(TFT_BLACK);
}

void CompositeDisplay::displayString(char* string, int x, int y){  

  //videoOut.waitForFrame();

  videoOut.setCursor(x, y + 20);
  videoOut.setTextWrap(false);
  videoOut.setTextColor(TFT_BLUE);
  Serial.println(string);
  videoOut.print(string);
  
}

void CompositeDisplay::drawRect(int x,int y, int h, int w, int  c){
  videoOut.drawRect(x,y,h,w,c);
}

void CompositeDisplay::setRotation(int r){
  videoOut.setRotation(r);
}

void CompositeDisplay::waitForFrame(){
  videoOut.waitForFrame();
}