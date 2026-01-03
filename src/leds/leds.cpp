#include "leds.h"

namespace
{
    template<uint8_t DataPin>
    CLEDController *getStaticController()
    {
        static WS2812B<DataPin, GRB> controller;
        return &controller;
    }

    CLEDController *controllerForPin(uint8_t pin)
    {
        switch (pin)
        {
        case 14:
            return getStaticController<14>();
        case 27:
            return getStaticController<27>();
        default:
            return nullptr;
        }
    }
}

LEDs::LEDs(uint8_t numLedsStrip1, uint8_t numLedsStrip2, uint8_t stripPin1, uint8_t stripPin2) : _numLedsStrip1(numLedsStrip1), _numLedsStrip2(numLedsStrip2), _stripPin1(stripPin1), _stripPin2(stripPin2)
{
}


void LEDs::init()
{
    pinMode(_stripPin1, OUTPUT);
    pinMode(_stripPin2, OUTPUT);
    _strip1Leds = new CRGB[_numLedsStrip1];
    _strip2Leds = new CRGB[_numLedsStrip2];

    CLEDController *strip1Controller = controllerForPin(_stripPin1);
    CLEDController *strip2Controller = controllerForPin(_stripPin2);

    if (strip1Controller != nullptr)
    {
        FastLED.addLeds(strip1Controller, _strip1Leds, _numLedsStrip1);
    }
    if (strip2Controller != nullptr)
    {
        FastLED.addLeds(strip2Controller, _strip2Leds, _numLedsStrip2);
    }

    if(strip1Controller != nullptr || strip2Controller != nullptr)
    {
        FastLED.setBrightness(_bringthness);
        FastLED.clear(true);
    }
}
