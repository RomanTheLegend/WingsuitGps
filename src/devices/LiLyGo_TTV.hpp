#ifdef TTV
#include "DisplayDevice.hpp"


class TTV : public DisplayDevice
{
  public:
    void init() override;
    void displayDigits(int current, int previous) override;
    void displayString(char* string, int x, int y) override;
    void displaySide() override;
    void displayTop() override;
    void clearScreen() override;
};
#endif