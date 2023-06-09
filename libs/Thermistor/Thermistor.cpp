#include "Thermistor.h"

Thermistor::Thermistor(){}
Thermistor::~Thermistor(){}

void Thermistor::init(uint8_t pin, float R1, float temp_gradient, float temp_offset)
{
    if (initialized)
        return;

    _pin = pin;
    _R1 = R1;
    _temp_gradient = temp_gradient;
    _temp_offset = temp_offset;

    Vo = 0;
    temperature = 0;
    
    pinMode(_pin,INPUT);
    analogReadResolution(10);

    initialized = true;
}

float Thermistor::getTemperature()
{
    if (!initialized)
        return NAN;

    float tot_temp=0;

    for (uint8_t i=0;i<10;i++)
    {
        Vo = analogRead(A0);

        if (Vo>1000)
            return NAN;

        R2 = _R1/(1023.0/Vo-1.0);
        temperature = (R2-_temp_offset)/_temp_gradient;

        tot_temp = tot_temp + temperature;
        delayMicroseconds(500);
    }   
   
    temperature = tot_temp/10;

    return temperature;
}