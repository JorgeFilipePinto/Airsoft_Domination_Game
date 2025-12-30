#include "buzzer.h"


Buzzer::Buzzer(uint8_t pin) : _pin(pin)
{
}


void Buzzer::init()
{
    pinMode(_pin, OUTPUT);
}


void Buzzer::buzz(unsigned int duration)
{
    currentMillis = millis();
    if(_buzzStartTime - currentMillis >= duration)
    {
        _buzzStartTime = currentMillis;
        digitalWrite(_pin, LOW);
        return;
    }
    digitalWrite(_pin, HIGH);
}


