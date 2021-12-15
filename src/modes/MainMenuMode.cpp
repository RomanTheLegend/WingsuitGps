#include "../devices/GhudDevice.hpp"
#include "../modules/ButtonInterface.hpp"
#include "ModeManager.hpp"
#include "MainMenuMode.hpp"


#define MENU_POSITIONS 4

char* menu_items[] = {
  "Horizontal speed",
  "Random speed",
  "Heading",
  "GPS stats"
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
  if (refreshMenuScreen){
    GhudDevice::clearScreen();
    GhudDevice::displayString("Select mode:", 10 , 10);
    for (int i=0; i< MENU_POSITIONS; i++){
      char buffer[25];
      if (i+1==cursor_position){
        sprintf(buffer, "* %s", menu_items[i]);
      }  else {
        sprintf(buffer, "  %s", menu_items[i]);
      }    
      //Serial.println(buffer);
      GhudDevice::displayString(buffer, 20 , 10 + (i+1)*20);
    }
    refreshMenuScreen=false;
  }
  //delay(250);
}
