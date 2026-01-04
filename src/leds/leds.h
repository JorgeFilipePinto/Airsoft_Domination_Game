#pragma once
#include <Arduino.h>
#include "FastLED.h"

class LEDs
{
public:
    LEDs(
        uint8_t numLedsStrip1,
        uint8_t numLedsStrip2,
        uint8_t stripPin1,
        uint8_t stripPin2);
    void init();
    void setQueue(QueueHandle_t queue);
    static void loop(void *parameter);
    void setColor();
    void turnOff();
    void blinkColor();
    void staticColor();

private:
    static LEDs *_instance;
    static QueueHandle_t _queue;
    CRGB *_strip1Leds;
    CRGB *_strip2Leds;
    uint8_t _numLedsStrip1;
    uint8_t _numLedsStrip2;
    uint8_t _stripPin1;
    uint8_t _stripPin2;
    uint8_t _bringthness = 80;
};