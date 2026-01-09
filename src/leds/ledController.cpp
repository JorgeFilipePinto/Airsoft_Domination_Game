#pragma once
#include <Arduino.h>
#include "FastLED.h"

enum LEDMODE {
    OFF,
    BLINK,
    STATIC,
    CHAINING
};

struct LedController {
    LEDMODE mode;
    uint8_t numberOfChainLeds = 1;
    CRGB color;
    uint16_t delay;
};