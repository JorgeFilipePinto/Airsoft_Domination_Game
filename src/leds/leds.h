#include <Arduino.h>
#pragma once


class LEDs
{
public:
    LEDs(uint8_t redPin, uint8_t greenPin, uint8_t bluePin);
    void init();
    void setColor(uint8_t red, uint8_t green, uint8_t blue);
    void turnOff();
    void blinkColor(uint8_t red, uint8_t green, uint8_t blue, unsigned int delayTime);
    void staticColor(uint8_t red, uint8_t green, uint8_t blue, unsigned int delayTime);
private:
    uint8_t _redPin;
    uint8_t _greenPin;
    uint8_t _bluePin;
};