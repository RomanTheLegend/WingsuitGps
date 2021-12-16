#include "../devices/GhudDevice.hpp"
#include "../modules/ButtonInterface.hpp"
#include "../modules/GpsInterface.hpp"
#include "ModeManager.hpp"
#include "GpsStatsMode.hpp"

bool statsNeedRefresh=true;
int prevSatelitesFound;
float fLatPrev, fLonPrev;


String floatToString(float val, float invalid, int prec);
String getDate();

void GpsStatsMode::init(){
  prevSatelitesFound=-1;  
}


void GpsStatsMode::processInput(ButtonEvent event){

}

void GpsStatsMode::display(){  

  float fLat, fLon;
  unsigned long age;


  int satelitesFound = GpsInterface::getGps()->satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 :  GpsInterface::getGps()->satellites();

  if (prevSatelitesFound != satelitesFound){
    statsNeedRefresh = true;
  };  
  if (statsNeedRefresh){
    GhudDevice::clearScreen();
    GhudDevice::displayString("GPS stats:", 10 , 10);    

    int i;
    char buffer[25];
    
    sprintf(buffer, "Satelites: %d", satelitesFound);
    GhudDevice::displayString(buffer, 20 , 10 + (i+1)*20);
    i++;
      /*
    sprintf(buffer, "Lat: %s", floatToString(fLat,TinyGPS::GPS_INVALID_F_ANGLE, 6 ));
    GhudDevice::displayString(buffer, 20 , 10 + (i+1)*20);
    i++;

    sprintf(buffer, "Lon: %s", floatToString(fLon,TinyGPS::GPS_INVALID_F_ANGLE, 6 ));
    GhudDevice::displayString(buffer, 20 , 10 + (i+1)*20);
    i++;
*/
    //sprintf(buffer, "Date: %s", getDate());
    //GhudDevice::displayString(buffer, 20 , 10 + (i+1)*20);
    i++;
    
      
    prevSatelitesFound = satelitesFound;
    fLatPrev = fLat;
    fLonPrev = fLon;
  }
  statsNeedRefresh=false;
    //delay(250);
}

String floatToString(float val, float invalid, int prec)
{
  char floatString[11];
  if (val == invalid)
  {
    sprintf(floatString, "<invalid data>");
  }
  else
  {
    dtostrf(val,11,prec,floatString);
  }
  return floatString;
}

String getDate(){
  int year;
  byte month, day, hour, minute, second, hundredths;
  unsigned long age;
  char dateString[32];
  GpsInterface::getGps()->crack_datetime(&year, &month, &day, &hour, &minute, &second, &hundredths, &age);
  if (age == TinyGPS::GPS_INVALID_AGE)
    sprintf(dateString, "<invalid data>");
  else
  {    
    sprintf(dateString, "%02d/%02d/%02d %02d:%02d:%02d ",
        month, day, year, hour, minute, second);
  }
return dateString;
}