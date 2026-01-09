#pragma once
#include <Arduino.h>
#include "FastLED.h"
#include "ledController.cpp"

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
    static void start(void *parameter);
    void loop();
    void setColor();
    void blinkColor();
    void turnOff();
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

    LEDMODE _currentMode = OFF;
    CRGB _currentColor = CRGB::White;
    uint16_t _currentDelay = 50;
    uint8_t _chainPosition = 0;
    uint8_t _currentNumberOfChainLeds = 1;

    void chainingEffect();
    void updateMode();
};