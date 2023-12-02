#include "ModeManager.hpp"
#include "MainMenuMode.cpp"
#include "RandomSpeedMode.cpp"
#include "HorizontalSpeedMode.cpp"
#include "HeadingMode.cpp"
#include "GpsStatsMode.cpp"
#include "CompetitionMode.cpp"
#include "RectanglesMode.cpp"


namespace ModeManager{

DisplayMode* const gMode[] = {
  new MainMenuMode(),
  new CompetitionMode(),
  new HorizontalSpeedMode(),
  //new VerticalSpeed(AnimationModeRoll::Left),
  //new TotalSpeed(AnimationModeRoll::Right),
  new RandomSpeedMode(),
  new HeadingMode(),
  new GpsStatsMode(),
  new RectanglesMode()
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
};