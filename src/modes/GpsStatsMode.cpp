#pragma once
#include "../devices/GhudDevice.hpp"
#include "../modules/ButtonInterface.hpp"
#include "../modules/GpsInterface.hpp"
// #include "ModeManager.hpp"
#include "esp_adc_cal.h"
#include "../config.h"
#include "DisplayMode.hpp"

class GpsStatsMode : public DisplayMode
{
private:
bool statsNeedRefresh=true;
int prevSatelitesFound;
float fLatPrev, fLonPrev;

int vref = 1100;
float VBAT= 0; // battery voltage from ESP32 ADC read


void displayGpsStats(){

//   float fLat, fLon;
//   unsigned long age;


//   int satelitesFound = GpsInterface::getGps()->satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 :  GpsInterface::getGps()->satellites();

//   if (prevSatelitesFound != satelitesFound){
//     statsNeedRefresh = true;
//   };  
//   if (statsNeedRefresh){
//     GhudDevice::clearScreen();
//     GhudDevice::displayString("GPS stats:", 10 , 10);    

//     int i;
//     char buffer[25];
    
//     sprintf(buffer, "Satelites: %d", satelitesFound);
//     GhudDevice::displayString(buffer, 20 , 10 + (i+1)*20);
//     i++;
//       /*
//     sprintf(buffer, "Lat: %s", floatToString(fLat,TinyGPS::GPS_INVALID_F_ANGLE, 6 ));
//     GhudDevice::displayString(buffer, 20 , 10 + (i+1)*20);
//     i++;

//     sprintf(buffer, "Lon: %s", floatToString(fLon,TinyGPS::GPS_INVALID_F_ANGLE, 6 ));
//     GhudDevice::displayString(buffer, 20 , 10 + (i+1)*20);
//     i++;
// */
//     //sprintf(buffer, "Date: %s", getDate());
//     //GhudDevice::displayString(buffer, 20 , 10 + (i+1)*20);
//     i++;
    
      
//     prevSatelitesFound = satelitesFound;
//     fLatPrev = fLat;
//     fLonPrev = fLon;
//   }
//   statsNeedRefresh=false;
//     //delay(250);
}

void displayBatteryStats(){
  int y=50;
  char buffer[25];
  char voltage[4];

  VBAT = (float)(analogRead(ADC_PIN)) * 3600 / 4095 * 2 / 1000;
  dtostrf(VBAT,4,2,voltage);
  sprintf(buffer, "Vbat = %s Volts", voltage );

  GhudDevice::displayString(buffer, 20 , y);     
}


String floatToString(float val, float invalid, int prec)
{
  char floatString[11];
  if (val == invalid)
  {
    sprintf(floatString, "n/a");
  }
  else
  {
    dtostrf(val,11,prec,floatString);
  }
  return floatString;
}


// String getDate(){
//   int year;
//   byte month, day, hour, minute, second, hundredths;
//   unsigned long age;
//   char dateString[32];
//   GpsInterface::getGps()->crack_datetime(&year, &month, &day, &hour, &minute, &second, &hundredths, &age);
//   if (age == TinyGPS::GPS_INVALID_AGE)
//     sprintf(dateString, "<invalid data>");
//   else
//   {    
//     sprintf(dateString, "%02d/%02d/%02d %02d:%02d:%02d ",
//         month, day, year, hour, minute, second);
//   }
//   return dateString;
// }

public:
void init(){
  prevSatelitesFound=-1;  


  esp_adc_cal_characteristics_t adc_chars;
  esp_adc_cal_value_t val_type = esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100, &adc_chars);    //Check type of calibration value used to characterize ADC
  if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF) {
      Serial.printf("eFuse Vref:%u mV", adc_chars.vref);
      vref = adc_chars.vref;
  } else if (val_type == ESP_ADC_CAL_VAL_EFUSE_TP) {
      Serial.printf("Two Point --> coeff_a:%umV coeff_b:%umV\n", adc_chars.coeff_a, adc_chars.coeff_b);
  } else {
      Serial.println("Default Vref: 1100mV");
  }

}


void processInput(ButtonEvent event){

}

void display(){  
  displayGpsStats();
  displayBatteryStats();
}


};