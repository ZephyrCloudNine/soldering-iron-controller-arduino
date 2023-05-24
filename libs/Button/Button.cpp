#include "Button.h"

Button::Button(){};
Button::~Button(){};

void Button::init(uint8_t pin)
{
    _pin = pin;
    pinMode(_pin,INPUT_PULLUP);

    short_press_timer_ms = 0;
    hold_press_timer_ms = 0;
    state = false;
}

bool Button::isShortPressed()
{
    if (digitalRead(_pin))
    {
        short_press_timer_ms = millis();
        state = false;
    }
    else if (!digitalRead(_pin) && !state)
    {
        if (millis()-short_press_timer_ms > SHORT_PRESS_DURATION_MS)
        {
            state = true;
            return true;
        }
    }

    return false;
}

bool Button::isHeld()
{
    if (digitalRead(_pin))
    {
        hold_press_timer_ms = millis();
    }
    else if (!digitalRead(_pin))
    {
        if (millis()-hold_press_timer_ms > HOLD_PRESS_DURATION_MS)
        {
            hold_press_timer_ms = millis();
            return true;
        }
    }

    return false;
}


