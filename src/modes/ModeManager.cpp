#include "ModeManager.hpp"
#include "MainMenuMode.hpp"
#include "RandomSpeedMode.hpp"
#include "HorizontalSpeedMode.hpp"
#include "HeadingMode.hpp"
#include "GpsStatsMode.hpp"


namespace ModeManager{

DisplayMode* const gMode[] = {
  new MainMenuMode(),
  new HorizontalSpeedMode(),
  //new VerticalSpeed(AnimationModeRoll::Left),
  //new TotalSpeed(AnimationModeRoll::Right),
  new RandomSpeedMode(),
  new HeadingMode(),
  new GpsStatsMode()
};

//const uint8_t cModeCount = 3;
Mode gCurrentMode = Mode::Menu;


void init()
{
  gMode[static_cast<uint8_t>(gCurrentMode)]->init();
}


void displayMode(Mode mode)
{
  Serial.print("Switching to mode:");Serial.println(mode);
  if (gCurrentMode != mode) {
    gCurrentMode = mode;
    gMode[static_cast<uint8_t>(gCurrentMode)]->init();
  }
  gMode[static_cast<uint8_t>(gCurrentMode)]->display();
}

void processInput(ButtonEvent event){
  Serial.print("Received event:");Serial.println(event);
  gMode[static_cast<uint8_t>(gCurrentMode)]->processInput(event);
}

  void progress(){
    gMode[static_cast<uint8_t>(gCurrentMode)]->display();
  }
}