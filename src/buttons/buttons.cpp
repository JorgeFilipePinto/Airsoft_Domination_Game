#include "buttons.h"


Buttons::Buttons(uint8_t pin1, uint8_t pin2, String contactType, String pin1name, String pin2name) : 
_pin1(pin1), _pin2(pin2), _contactType(contactType), _pin1name(pin1name), _pin2name(pin2name)
{
}


void Buttons::init()
{
    if (_contactType == "NC")
    {
        pinMode(_pin1, INPUT_PULLDOWN);
        pinMode(_pin2, INPUT_PULLDOWN);
    }
    else if (_contactType == "NO")
    {
        pinMode(_pin1, INPUT_PULLUP);
        pinMode(_pin2, INPUT_PULLUP);
    }
}


bool Buttons::isPressed(uint8_t buttonNumber)
{
    if (_contactType == "NC")
    {
        return digitalRead(buttonNumber) == HIGH;

    }
    if (_contactType == "NO")
    {
        return digitalRead(buttonNumber) == LOW;
    }
    return false;
}


bool Buttons::longPressed(uint8_t buttonNumber, unsigned long duration)
{
    unsigned long pressedTime = 0;
    while (isPressed(buttonNumber))
    {
        delay(10);
        pressedTime += 10;
        if (pressedTime >= duration)
        {
            return true;
        }
    }
    return false;
}