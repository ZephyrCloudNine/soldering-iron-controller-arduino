#ifndef NTC_THERMISTOR_H
#define NTC_THERMISTOR_H

#include "Arduino.h"

class NTCThermistor
{
    public:

        NTCThermistor();
        ~NTCThermistor();
        void init(uint8_t pin, float R1, float temp_gradient, float temp_offset);
        float getTemperature();
        
    
    private:

        bool initialized=false;
        uint8_t _pin;
        float Vo;
        float _R1;
        float R2;
        float _temp_gradient;
        float _temp_offset;
        float temperature;
        

};

#endif