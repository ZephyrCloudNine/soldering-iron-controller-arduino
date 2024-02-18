#include "HeatingElement.h"

HeatingElement::HeatingElement(){};
HeatingElement::~HeatingElement(){};

void HeatingElement::init(uint8_t pin)
{
    if (initialized)
        return;

    _pin = pin;
    _duty = 0;

    pid.begin();

    // pinMode(_pin,OUTPUT);
    // digitalWrite(_pin,LOW);

    ledcSetup(CHANNEL,FREQ,RESOLUTION);
    ledcAttachPin(_pin,CHANNEL);

    initialized = true;
}

void HeatingElement::setPIDconsts(uint8_t Kp, uint8_t Ki, uint8_t Kd,
                                    uint16_t lower_lim, uint16_t upper_lim)
{
    if (!initialized)
        return;

    pid.tune(Kp,Ki,Kd);
    pid.limit(lower_lim,upper_lim);
}

void HeatingElement::setTarget(uint16_t target)
{
    if (!initialized)
        return;

    pid.setpoint(target);
}

void HeatingElement::enable()
{
    if (!initialized)
        return;

    digitalWrite(_pin,HIGH);
}

void HeatingElement::disable()
{
    if (!initialized)
        return;

    digitalWrite(_pin,LOW);
}

void HeatingElement::drive(uint16_t duty)
{
    if (!initialized)
        return;

    _duty = duty;
    ledcWrite(CHANNEL, _duty);
}

void HeatingElement::runPID(uint16_t current_val)
{
    if (!initialized)
        return;

    _duty = pid.compute(current_val);
    ledcWrite(CHANNEL, _duty);
}

uint16_t HeatingElement::getDuty()
{
    if (!initialized)
        return (uint16_t)NAN;

    return _duty;
}
