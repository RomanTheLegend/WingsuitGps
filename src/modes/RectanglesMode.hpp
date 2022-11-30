#include "DisplayMode.hpp"

class RectanglesMode : public DisplayMode
{
public: // Implement AnimationMode
  void init() override;
  void processInput(ButtonEvent event) override;
  void display() override;
};
