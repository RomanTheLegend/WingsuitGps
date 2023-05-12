#include "../config.h"
#include "../devices/GhudDevice.hpp"
#include "../modules/ButtonInterface.hpp"
#include "DisplayMode.hpp"
#include "ModeManager.hpp"

#define MENU_POSITIONS 6
#define MAX_ITEMS_DISPLAYED 4

char* menu_items[] = {
  "Competition", 
  "Horizontal",
  "Random speed",
  "Heading",
  "GPS stats",
  "Rectangles"
};

class MainMenuMode : public DisplayMode
{

private:

byte cursor_position=1;
bool refreshMenuScreen=true;
int start_index = 0;
int end_index = MAX_ITEMS_DISPLAYED;

public:


void init(){
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
        


void processInput(ButtonEvent event){
  switch (event) {
  case UP:
      if (cursor_position > 1){
        cursor_position--;
      } else {
        cursor_position = MENU_POSITIONS;
        start_index = MENU_POSITIONS - MAX_ITEMS_DISPLAYED;
        end_index = MENU_POSITIONS;
      }
      refreshMenuScreen=true;
     break;
  case DOWN:
      if (cursor_position < MENU_POSITIONS){
        cursor_position++;
      } else {
        cursor_position=1;
        start_index=0;
        end_index = MAX_ITEMS_DISPLAYED;
      }

      refreshMenuScreen=true;
    break;
  case ENTER:
      ModeManager::displayMode((Mode)cursor_position);
      break;
  }
}


void display(){
  GhudDevice::waitForFrame();
  if (refreshMenuScreen){
    GhudDevice::clearScreen();
    GhudDevice::displayString("Select mode:", 22 , 10);



    if ( cursor_position <= start_index && start_index > 0 ){
      start_index--; 
      end_index--;
    }
    if ( cursor_position > end_index ){
      start_index++;
      end_index++;
    }

      Serial.println();
      Serial.print("\t\tcursor_position "); Serial.println(cursor_position);
      Serial.print("\t\tstart_index "); Serial.println(start_index);
      Serial.print("\t\tend_index "); Serial.println(end_index);
      Serial.println();
    
    // int end = 3;
    for (int i=start_index; i< end_index; i++){
      char buffer[25];
      if (i+1==cursor_position){
        sprintf(buffer, "* %s", menu_items[i]);
      }  else {
        sprintf(buffer, "  %s", menu_items[i]);
      }    
      Serial.println(buffer);
      // GhudDevice::displayString(buffer, 20 + MENU_SPACING_X, 10 + (i+1)*MENU_SPACING_Y);
      GhudDevice::displayString(buffer, 20 + MENU_SPACING_X, 10 + (i-start_index+1)*MENU_SPACING_Y);

    }
    refreshMenuScreen=false;    
  }
  //delay(250);
}




};
