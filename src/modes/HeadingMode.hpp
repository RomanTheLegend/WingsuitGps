#include "DisplayMode.hpp"

class HeadingMode : public DisplayMode
{
public: // Implement AnimationMode
  void init() override;
  void processInput(ButtonEvent event) override;
  void display() override;
};
