#include <HardwareSerial.h>
//#include <EEPROM.h>
#include "config.h"
#include "devices/GhudDevice.hpp"
#include "modes/ModeManager.hpp"
#include "modules/ButtonInterface.hpp"
#include "modules/GpsInterface.hpp"
#include "modules/BluetoothInterface.hpp"

//#define EEPROM_SIZE   1


//https://github.com/LuckyResistor/guide-modular-firmware/blob/master/fade_demo_08/fade_demo_08.ino

void processButtonInput();

void setup()
{
  Serial.begin(115200);
  Serial.println("Starting up");
  GhudDevice::init();
  delay(100);
//  GpsInterface::init( TX_PIN , RX_PIN );
  //BluetoothInterface::init();
  ButtonInterface::init();
  ButtonInterface::setCallback(&processButtonInput);
  ModeManager::init();
  
  //EEPROM.begin(EEPROM_SIZE);
  //clickCounter = EEPROM.read(0);


  Serial.println("G-HUD initialization done");
  ModeManager::displayMode((Mode)0);
}




void loop()
{ 
  ModeManager::progress();
//  GpsInterface::loop();
  processButtonInput();
}


void processButtonInput(){

  ButtonEvent event = ButtonInterface::getLastEvent();
  if (event != NONE){
      switch (event){
        case UP:
        case DOWN:
        case ENTER:
          ModeManager::processInput(event);
        break;
        case EXIT:
          ModeManager::displayMode((Mode)0);
        break;
      }
  }
}

