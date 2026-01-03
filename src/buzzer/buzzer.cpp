#include "buzzer.h"


Buzzer::Buzzer(uint8_t pin) : _pin(pin)
{
}


void Buzzer::init()
{
    pinMode(_pin, OUTPUT);
    test();
}


void Buzzer::beep()
{
    digitalWrite(_pin, HIGH);
    delay(100);
    digitalWrite(_pin, LOW);
}


void Buzzer::warning()
{
    digitalWrite(_pin, HIGH);
    delay(1000);
    digitalWrite(_pin, LOW);
}


void Buzzer::test()
{
    for (int i = 0; i < 3; i++)
    {
        digitalWrite(_pin, HIGH);
        delay(200);
        digitalWrite(_pin, LOW);
        delay(200);
    }
}
