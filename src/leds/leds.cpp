#include "leds.h"

LEDs::LEDs(uint8_t redPin, uint8_t greenPin, uint8_t bluePin) : _redPin(redPin), _greenPin(greenPin), _bluePin(bluePin)
{
}


void LEDs::init()
{
    pinMode(_redPin, OUTPUT);
    pinMode(_greenPin, OUTPUT);
    pinMode(_bluePin, OUTPUT);
    turnOff();
}


void LEDs::setColor(uint8_t red, uint8_t green, uint8_t blue)
{
    digitalWrite(_redPin, red);
    digitalWrite(_greenPin, green);
    digitalWrite(_bluePin, blue);
}

void LEDs::blinkColor(uint8_t red, uint8_t green, uint8_t blue, unsigned int delayTime)
{
    setColor(red, green, blue);
    delay(delayTime);
    turnOff();
    delay(delayTime);
}


void LEDs::staticColor(uint8_t red, uint8_t green, uint8_t blue, unsigned int delayTime)
{
    setColor(red, green, blue);
    delay(delayTime);
}


void LEDs::turnOff()
{
    setColor(0, 0, 0);
}