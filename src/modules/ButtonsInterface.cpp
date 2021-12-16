#include <Button2.h>
#include "ButtonInterface.hpp"
#include "../devices/LiLyGo_TGO_pin_setup.h"


namespace ButtonInterface {

    Button2 button_up(BUTTON_UP);
    Button2 button_enter(BUTTON_ENTER);
    Button2 button_down(BUTTON_DOWN);

    bool longpress=false;

    ButtonEvent event=NONE;

    Function gCallback;

    void init()
    {

        button_up.setLongClickTime(2000);
        button_up.setLongClickDetectedHandler([](Button2 & b) {
            Serial.println("Exit");
            event = EXIT;
            longpress=true;
        });
        button_up.setTapHandler([](Button2 & b) {
            Serial.println("Up"); 
            if (!longpress){
                event = UP;
            } else {
                longpress=false;
            }
        });


        button_enter.setLongClickTime(2000);
        button_enter.setLongClickDetectedHandler([](Button2 & b) {
            Serial.println("Enter");
            event = ENTER;
            longpress=true;            
        });

        button_enter.setTapHandler([](Button2 & b) {
            Serial.println("Down");
            if (!longpress){
                event = DOWN;
            } else {
                longpress=false;
            }
        });

        /*
        button_down.setTapHandler([](Button2 & b) {
            Serial.println("Down");
            //move_cursor(2);
        });
        */
    }


    void setCallback(Function fn)
    {
        gCallback = fn;
    }

    void button_loop()
    {
        button_up.loop();
        button_enter.loop();
        button_down.loop();
    }

    ButtonEvent getLastEvent(){
        button_loop();
        ButtonEvent last_event = event;
        event = NONE;
        return last_event;
    }

}