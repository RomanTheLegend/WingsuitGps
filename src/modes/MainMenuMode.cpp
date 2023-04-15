#include "../config.h"
#include "../devices/GhudDevice.hpp"
#include "../modules/ButtonInterface.hpp"
#include "ModeManager.hpp"
#include "MainMenuMode.hpp"


#define MENU_POSITIONS 6
#define MAX_ITEMS_DISPLAYED 4

char* menu_items[] = {
  "Competition"
  "Horizontal",
  "Random speed",
  "Heading",
  "GPS stats",
  "Rectangles"
};

byte cursor_position=1;
bool refreshMenuScreen=true;
int menu_display_start = 0;
int menu_display_end = MAX_ITEMS_DISPLAYED;

void MainMenuMode::init(){
  refreshMenuScreen = true;
}




        // case DOWN:
        //     if (cursor_position < MENU_POSITIONS - 1) {
        //         cursor_position++;
        //     }
        //     if (cursor_position > bottom_item) {
        //         top_item++;
        //         bottom_item++;
        //     }
        // case UP:
        //     if (cursor_position > 0) {
        //         cursor_position--;
        //     }
        //     if (cursor_position < top_item) {
        //         top_item--;
        //         bottom_item--;
        //     }
        


void MainMenuMode::processInput(ButtonEvent event){
  switch (event) {
  case UP:
      if (cursor_position > 1){
        cursor_position--;
      } else {
        cursor_position=MENU_POSITIONS;
        menu_display_start = MENU_POSITIONS - MAX_ITEMS_DISPLAYED -1;
        menu_display_end = MENU_POSITIONS-1;
      }

      if ( cursor_position < menu_display_start){
        menu_display_start --;
        menu_display_end --;
      }
      
      refreshMenuScreen=true;
     break;
  case DOWN:
      Serial.println(cursor_position);
      if (cursor_position < MENU_POSITIONS){
        cursor_position++;
      } else {
        cursor_position=1;
        menu_display_start = 0;
        menu_display_end = MAX_ITEMS_DISPLAYED;
      }
  
      if ( cursor_position >= menu_display_end){
        menu_display_start++;
        menu_display_end++;
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

    
    // int end = 3;
    for (int i=menu_display_start; i< menu_display_end; i++){
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
