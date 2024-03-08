/***************************************************************************
**                                                                        **
**  Copyright 2024 Roman Kharytonov                                       **
**                                                                        **
**  This program is free software: you can redistribute it and/or modify  **
**  it under the terms of the GNU Affero General Public License as        **
**  published by the Free Software Foundation, either version 3           **
**  of the License, or (at your option) any later version.                **
**                                                                        **
**  This program is distributed in the hope that it will be useful,       **
**  but WITHOUT ANY WARRANTY; without even the implied warranty of        **
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         **
**  GNU General Public License for more details.                          **
**                                                                        **
**  You should have received a copy of the GNU Affero General Public      **
**  License along with this program.  If not, see                         **
**  <http://www.gnu.org/licenses/>                                        **
****************************************************************************
**  Contact: Roman Kharytonov                                             **
**  GitHub: https://github.com/RomanTheLegend/WingsuitGps                 **
****************************************************************************/

#include <HardwareSerial.h>
//#include <EEPROM.h>
#include "config.h"
#include "devices/GhudDevice.hpp"
#include "modes/ModeManager.hpp"
#include "modules/ButtonInterface.hpp"
#include "modules/GpsInterface.hpp"
#include "modules/BluetoothInterface.hpp"
#include "modules/DataProvider.cpp"

//#define EEPROM_SIZE   1


//https://github.com/LuckyResistor/guide-modular-firmware/blob/master/fade_demo_08/fade_demo_08.ino

void processButtonInput();

void setup()
{
  Serial.begin(115200);
  Serial.println("Starting up");
  GhudDevice::init();
  delay(100);
  // GpsInterface::init( TX_PIN , RX_PIN );
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
  DataProvider::getInstance().refresh();
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

