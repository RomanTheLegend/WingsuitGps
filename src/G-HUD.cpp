#include <Arduino.h>
#include <HardwareSerial.h>
#include <EEPROM.h>
#include "devices/GhudDevice.hpp"
#include "modes/ModeManager.hpp"
#include "modules/ButtonInterface.hpp"
#include "modules/GpsInterface.hpp"

#define EEPROM_SIZE   1
#define TX_PIN 27
#define RX_PIN 26

//https://github.com/LuckyResistor/guide-modular-firmware/blob/master/fade_demo_08/fade_demo_08.ino

void processButtonInput();

void setup()
{
  Serial.begin(115200);
  Serial.println("Starting up");
  GhudDevice::init();
  GpsInterface::init( TX_PIN , RX_PIN );
  ButtonInterface::init();
  ModeManager::init();
  ButtonInterface::setCallback(&processButtonInput);


  //EEPROM.begin(EEPROM_SIZE);
  //clickCounter = EEPROM.read(0);
  

  //button_init();
 

  Serial.println("initialization done.");
  Serial.println("Ready");
  ModeManager::displayMode((Mode)0);
}




void loop()
{ 
  ModeManager::progress();
  GpsInterface::loop();
  processButtonInput();
}


void processButtonInput(){
  ButtonEvent event = ButtonInterface::getLastEvent();
  if (event != NONE){
      switch (event){
        case ButtonEvent::UP:
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

