
#include <ESP_8_BIT_GFX.h>
#include "../config.h"
#include "CompositeDisplay.hpp"


//https://rop.nl/truetype2gfx/
//https://tchapi.github.io/Adafruit-GFX-Font-Customiser/
#include "../../fonts/Inconsolata_Bold30pt7b.h"
#include "../../fonts/NotoSans_Bold10pt7b.h"


#define _DEBUG

ESP_8_BIT_GFX videoOut(true /* = NTSC */, 8 /* = RGB332 color */);

// Color picker:
// https://roger-random.github.io/RGB332_color_wheel_three.js/

int GLOBAL_COLOR = TFT_MAROON;

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
int c_x=30; int c_y=130;
  videoOut.setFont(&Inconsolata_Bold30pt7b);

  if((previous != current)){
    if (previous !=0){
      videoOut.setTextColor(TFT_BLACK, TFT_BLACK);
      videoOut.setCursor(c_x, c_y);
      videoOut.print(String(previous));
    } else {
      videoOut.fillScreen(0);
    }
    videoOut.setTextColor(0x54, TFT_BLACK);
    videoOut.setTextWrap(false);
    // videoOut.setTextSize(10);
    // videoOut.setTextColor(TFT_BLUE);
    videoOut.setCursor(c_x, c_y);
    // Serial.println(current);
    videoOut.print(current);
  }

}

void CompositeDisplay::clearScreen(){
  videoOut.fillScreen(TFT_BLACK);
}

void CompositeDisplay::displayString(char* string, int color, int x, int y){  

  GLOBAL_COLOR = color;
  //videoOut.waitForFrame();
  videoOut.setFont(&NotoSans_Bold10pt7b);
  videoOut.setCursor(x, y + 20);
  videoOut.setTextWrap(false);
  videoOut.setTextColor(GLOBAL_COLOR);
  // Serial.println(string);
  videoOut.print(string);
  
}


void CompositeDisplay::displayString(char* string, int x, int y){  

  //videoOut.waitForFrame();
  videoOut.setFont(&NotoSans_Bold10pt7b);
  videoOut.setCursor(x, y + 20);
  videoOut.setTextWrap(false);
  videoOut.setTextColor(TFT_BLUE);
  // Serial.println(string);
  videoOut.print(string);
  
}

void CompositeDisplay::drawRect(int x,int y, int h, int w, int  c){
  videoOut.drawRect(x,y,h,w,c);
}

void CompositeDisplay::drawLine(int x,int y, int x1, int y1, int  c ){
  videoOut.drawLine(x,y,x1,y1,c);
}


void CompositeDisplay::setRotation(int r){
  videoOut.setRotation(r);
}

void CompositeDisplay::waitForFrame(){
  videoOut.waitForFrame();
}