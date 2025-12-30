#include <Arduino.h>
#pragma once


class Buttons
{
public:
    Buttons(uint8_t pin1, uint8_t pin2, String contactType, String pin1name, String pin2name);
    void init();
    bool isPressed(uint8_t buttonNumber);
    bool longPressed(uint8_t buttonNumber, unsigned long duration);
    
private:
    uint8_t _pin1;
    uint8_t _pin2;
    String _contactType;
    String _pin1name;
    String _pin2name;
};