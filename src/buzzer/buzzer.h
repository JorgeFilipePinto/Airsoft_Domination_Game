#include <Arduino.h>
#pragma once



class Buzzer
{
public:
    Buzzer(uint8_t pin);
    void init();
    void beep();
    void warning();
private:
    uint8_t _pin;
    unsigned long _buzzStartTime;
    unsigned long currentMillis;
    void test();
};