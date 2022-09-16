
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

#ifdef USE_TTV
    #include <U8g2lib.h>
    #include "devices/LiLyGo_TTV.hpp"
    #define MENU_SPACING_X  10
    #define MENU_SPACING_Y  20
    #define TX_PIN          21
    #define RX_PIN          22
    #define BUTTON_UP     12
    #define BUTTON_ENTER  13
    #define BUTTON_DOWN   17
    #define ADC_PIN             34
#endif

