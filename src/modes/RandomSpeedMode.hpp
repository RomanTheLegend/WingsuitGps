#include "DisplayMode.hpp"

class RandomSpeedMode : public DisplayMode
{
public: // Implement AnimationMode
  void init() override;
  void processInput(ButtonEvent event) override;
  void display() override;
};
