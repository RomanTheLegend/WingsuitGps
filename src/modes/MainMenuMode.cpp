#include "../config.h"
#include "../devices/GhudDevice.hpp"
#include "../modules/ButtonInterface.hpp"
#include "ModeManager.hpp"
#include "MainMenuMode.hpp"


#define MENU_POSITIONS 5

char* menu_items[] = {
  "Horizontal",
  "Random speed",
  "Heading",
  "GPS stats",
  "Rectangles"
};

byte cursor_position=1;
bool refreshMenuScreen=true;

void MainMenuMode::init(){
    refreshMenuScreen = true;
}


void MainMenuMode::processInput(ButtonEvent event){
  switch (event) {
  case DOWN:
      Serial.println(cursor_position);    
      if (cursor_position < MENU_POSITIONS){
        cursor_position++;
      } else {
        cursor_position=1;
      }
      refreshMenuScreen=true;
    break;
  case UP:
      if (cursor_position > 1){
        cursor_position--;
      } else {
        cursor_position=MENU_POSITIONS;
      }
      refreshMenuScreen=true;
     break;
  case ENTER:
      ModeManager::displayMode((Mode)cursor_position);
      break;
  }
}


void MainMenuMode::display(){
  GhudDevice::waitForFrame();
  if (refreshMenuScreen){
    GhudDevice::clearScreen();
    GhudDevice::displayString("Select mode:", 22 , 10);
    for (int i=0; i< MENU_POSITIONS; i++){
      char buffer[25];
      if (i+1==cursor_position){
        sprintf(buffer, "* %s", menu_items[i]);
      }  else {
        sprintf(buffer, "  %s", menu_items[i]);
      }    
      //Serial.println(buffer);
      GhudDevice::displayString(buffer, 20 + MENU_SPACING_X, 10 + (i+1)*MENU_SPACING_Y);
    }
    refreshMenuScreen=false;    
  }
  //delay(250);
}
