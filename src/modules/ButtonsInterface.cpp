#include <Button2.h>
#include "ButtonInterface.hpp"
#include "../config.h"


namespace ButtonInterface {

    Button2 button_up(BUTTON_UP);
    Button2 button_down(BUTTON_DOWN);


    #ifdef BUTTON_ENTER
        Button2 button_enter(BUTTON_ENTER);
    #endif

    bool longpress=false;

    ButtonEvent event=NONE;

    Function gCallback;

    void init()
    {
    
    #ifdef BUTTON_ENTER
        button_enter.setTapHandler([](Button2 & b) {
            Serial.println("Enter"); 
            if (!longpress){
                event = ENTER;
            } else {
                longpress=false;
            }
        });

        button_enter.setLongClickTime(2000);
        button_enter.setLongClickDetectedHandler([](Button2 & b) {
            Serial.println("Exit");
            event = EXIT;
            longpress=true;
        });
    #else
        button_up.setLongClickTime(2000);
        button_up.setLongClickDetectedHandler([](Button2 & b) {
            Serial.println("Exit");
            event = EXIT;
            longpress=true;
        });

        button_down.setLongClickTime(2000);
        button_down.setLongClickDetectedHandler([](Button2 & b) {
            Serial.println("Enter");
            event = ENTER;
            longpress=true;            
        });
    #endif

        button_up.setTapHandler([](Button2 & b) {
            Serial.println("Up"); 
            if (!longpress){
                event = UP;
            } else {
                longpress=false;
            }
        });

        button_down.setTapHandler([](Button2 & b) {
            Serial.println("Down");
            if (!longpress){
                event = DOWN;
            } else {
                longpress=false;
            }
        });
        Serial.println("Buttons interface initialized");
    }


    void setCallback(Function fn)
    {
        gCallback = fn;
    }

    void button_loop()
    {
    #ifdef BUTTON_ENTER
        button_enter.loop();
    #endif
        button_up.loop();
        button_down.loop();
    }

    ButtonEvent getLastEvent(){
        button_loop();
        ButtonEvent last_event = event;
        event = NONE;
        return last_event;
    }

}