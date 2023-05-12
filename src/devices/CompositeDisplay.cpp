#pragma once
#include <ESP_8_BIT_GFX.h>
#include "../config.h"
#include "DisplayDevice.hpp"
#include <math.h>

//https://rop.nl/truetype2gfx/
//https://tchapi.github.io/Adafruit-GFX-Font-Customiser/
#include "../../fonts/Inconsolata_Bold30pt7b.h"
#include "../../fonts/NotoSans_Bold10pt7b.h"


#define _DEBUG



// Color picker:
// https://roger-random.github.io/RGB332_color_wheel_three.js/



class CompositeDisplay : public DisplayDevice
{
private: 
int DEFAULT_COLOR = TFT_MAROON;
int GLOBAL_COLOR = DEFAULT_COLOR;

ESP_8_BIT_GFX* videoOut = new ESP_8_BIT_GFX(true /* = NTSC */, 8 /* = RGB332 color */);

public:
void init()
{
  videoOut->begin();
  videoOut->setRotation(3);
  videoOut->setTextSize(2);
  videoOut->copyAfterSwap = true;
  Serial.println("Composite Video initiated");
}


  void setFont(int id){
    switch (id){
      case 1: videoOut->setFont(); break;
      case 2: videoOut->setFont(&Inconsolata_Bold30pt7b);break;
      case 3: videoOut->setFont(&NotoSans_Bold10pt7b);break;
    }

  }

  void setColor(int id){
    videoOut->setTextColor(id);
    GLOBAL_COLOR = id;
  }

  void displayTop() {

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


void displaySide() {
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



void displayDigits(int current, int previous){
int c_x=30; int c_y=130;

  if((previous != current)){
    if (previous !=0){
      videoOut->setTextColor(TFT_BLACK, TFT_BLACK);
      videoOut->setCursor(c_x, c_y);
      videoOut->print(String(previous));
    } else {
      videoOut->fillScreen(0);
    }
    videoOut->setTextColor(0x54, TFT_BLACK);
    videoOut->setTextWrap(false);
    // videoOut->setTextSize(10);
    // videoOut->setTextColor(TFT_BLUE);
    videoOut->setCursor(c_x, c_y);
    // Serial.println(current);
    videoOut->print(current);
  }

}

void clearScreen(){
  videoOut->fillScreen(TFT_BLACK);
}

void displayString(char* string, int color, int x, int y){  

  GLOBAL_COLOR = color;
  //videoOut->waitForFrame();
  videoOut->setCursor(x, y + 20);
  videoOut->setTextWrap(false);
  videoOut->setTextColor(GLOBAL_COLOR);
  // Serial.println(string);
  videoOut->print(string);
  
}


void displayString(char* string, int x, int y){  

  //videoOut->waitForFrame();
  // videoOut->setFont(&NotoSans_Bold10pt7b);
  videoOut->setCursor(x, y + 20);
  videoOut->setTextWrap(false);
  videoOut->setTextColor(TFT_BLUE);
  // Serial.println(string);
  videoOut->print(string);
  
}

void drawRect(int x,int y, int h, int w, int  c){
  videoOut->drawRect(x,y,h,w,c);
}

void drawLine(int x,int y, int x1, int y1, int  c ){
  videoOut->drawLine(x,y,x1,y1,c);
}

void drawArrow(int cX, int cY, int angle ){
  int c = GLOBAL_COLOR;
  int m = 3;
  int x1, x2, x3, x4 ,x5, x6 ,x7;
  int y1, y2, y3, y4, y5, y6, y7;

  double radians = angle * M_PI / 180.0;
  double sin_angle = sin(radians);
  double cos_angle = cos(radians);


  x1 = - 2*m;
  x2 = - 2*m;
  x3 = - 4*m;
  x4 =   0 ;
  x5 = + 4*m;
  x6 = + 2*m;
  x7 = + 2*m;

  y1 = + 5*m;
  y2 = - 2*m;
  y3 = - 2*m;
  y4 = - 7*m;
  y5 = - 2*m;
  y6 = - 2*m;
  y7 = + 5*m;


  double new_x1 = x1 * cos_angle - y1 * sin_angle + cX;
  double new_y1 = x1 * sin_angle + y1 * cos_angle + cY;
  double new_x2 = x2 * cos_angle - y2 * sin_angle + cX;
  double new_y2 = x2 * sin_angle + y2 * cos_angle + cY;
  double new_x3 = x3 * cos_angle - y3 * sin_angle + cX;
  double new_y3 = x3 * sin_angle + y3 * cos_angle + cY;
  double new_x4 = x4 * cos_angle - y4 * sin_angle + cX;
  double new_y4 = x4 * sin_angle + y4 * cos_angle + cY;
  double new_x5 = x5 * cos_angle - y5 * sin_angle + cX;
  double new_y5 = x5 * sin_angle + y5 * cos_angle + cY;
  double new_x6 = x6 * cos_angle - y6 * sin_angle + cX;
  double new_y6 = x6 * sin_angle + y6 * cos_angle + cY;
  double new_x7 = x7 * cos_angle - y7 * sin_angle + cX;
  double new_y7 = x7 * sin_angle + y7 * cos_angle + cY;



  videoOut->drawLine(new_x1,new_y1,new_x2,new_y2,c);
  videoOut->drawLine(new_x2,new_y2,new_x3,new_y3,c);
  videoOut->drawLine(new_x3,new_y3,new_x4,new_y4,c);
  videoOut->drawLine(new_x4,new_y4,new_x5,new_y5,c);
  videoOut->drawLine(new_x5,new_y5,new_x6,new_y6,c);
  videoOut->drawLine(new_x6,new_y6,new_x7,new_y7,c);
  videoOut->drawLine(new_x7,new_y7,new_x1,new_y1,c);

}


void setRotation(int r){
  videoOut->setRotation(r);
}

void waitForFrame(){
  videoOut->waitForFrame();
}

};