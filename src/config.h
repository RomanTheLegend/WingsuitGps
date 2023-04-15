
//Uncomment device to be used
#define USE_TGO
//#define USE_TTV

#ifdef USE_TGO
    #include <TFT_eSPI.h>
    #include "devices/CompositeDisplay.hpp"
    #define MENU_SPACING_X  10
    #define MENU_SPACING_Y  20
    #define TX_PIN 27
    #define RX_PIN 26
    #define BUTTON_UP     35
    #define BUTTON_DOWN   0
    #define ADC_EN              14  //ADC_EN is the ADC detection enable port
    #define ADC_PIN             34
#endif

// static enum fgHudFonts = [
//     SYSTEM,

// ] 


