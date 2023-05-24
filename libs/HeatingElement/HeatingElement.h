#ifndef HEATING_ELEMENT_H
#define HEATING_ELEMENT_H

#include "Arduino.h"
#include "PIDController.h"

class HeatingElement
{
    public:

        HeatingElement();
        ~HeatingElement();
        void init(uint8_t pin);
        void setPIDconsts(uint8_t Kp, uint8_t Ki, uint8_t Kd,
                            uint16_t lower_lim, uint16_t upper_lim);
        void setTarget(uint16_t target);
        void enable();
        void disable();
        void drive(uint16_t duty);
        void runPID(uint16_t current_val);
        uint16_t getDuty();
    
    private:
        
        PIDController pid;
        bool initialized=false;        
        uint8_t _pin;
        uint16_t _duty;
};

#endif