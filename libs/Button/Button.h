#ifndef BUTTON_H
#define BUTTON_H

#include "Arduino.h"

#define SHORT_PRESS_DURATION_MS 150
#define HOLD_PRESS_DURATION_MS 75

class Button
{
    public:

        Button();
        ~Button();
        void init(uint8_t pin);
        bool isShortPressed();
        bool isHeld();

    private:

        uint8_t _pin;
        uint32_t short_press_timer_ms;
        uint32_t hold_press_timer_ms;
        bool state;
};

#endif