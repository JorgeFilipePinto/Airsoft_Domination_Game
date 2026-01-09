#include "leds.h"

LEDs *LEDs::_instance = nullptr;
QueueHandle_t LEDs::_queue = nullptr;

LEDs::LEDs(uint8_t numLedsStrip1, uint8_t numLedsStrip2, uint8_t stripPin1, uint8_t stripPin2) : _numLedsStrip1(numLedsStrip1), _numLedsStrip2(numLedsStrip2), _stripPin1(stripPin1), _stripPin2(stripPin2) {}

void LEDs::init()
{
    _instance = this;
    pinMode(_stripPin1, OUTPUT);
    pinMode(_stripPin2, OUTPUT);
    _strip1Leds = new CRGB[_numLedsStrip1];
    _strip2Leds = new CRGB[_numLedsStrip2];
    FastLED.addLeds<NEOPIXEL, 14>(_strip2Leds, _numLedsStrip2);
    FastLED.addLeds<NEOPIXEL, 17>(_strip1Leds, _numLedsStrip1);
    FastLED.setBrightness(_bringthness);
    Serial.println("[LEDs::init] LEDs initialized.");
}

void LEDs::setQueue(QueueHandle_t queue)
{
    _queue = queue;
}

void LEDs::start(void *parameter)
{
    Serial.println("[LEDs::start] LED task started.");
    while (true)
    {
        if (_instance != nullptr)
        {
            // Check for new commands from queue
            if (_queue != nullptr && uxQueueMessagesWaiting(_queue) > 0)
            {
                _instance->updateMode();
            }

            // Execute current mode
            _instance->loop();
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void LEDs::loop()
{
    switch (_currentMode)
    {
    case OFF:
        turnOff();
        delay(100);
        break;
    case BLINK:
        blinkColor();
        break;
    case STATIC:
        staticColor();
        delay(100);
        break;
    case CHAINING:
        chainingEffect();
        break;
    default:
        delay(100);
        break;
    }
}

void LEDs::updateMode()
{
    LedController ledController;
    if (xQueueReceive(_queue, &ledController, 0) == pdTRUE)
    {
        Serial.println("[LEDs::updateMode] New mode received: " + String(ledController.mode));
        _currentMode = ledController.mode;
        _currentColor = ledController.color;
        _currentDelay = ledController.delay;
        _chainPosition = 0;
        _currentNumberOfChainLeds = ledController.numberOfChainLeds;
    }
}


void LEDs::chainingEffect()
{
    // Apaga todos os LEDs
    fill_solid(_strip1Leds, _numLedsStrip1, CRGB::Black);
    fill_solid(_strip2Leds, _numLedsStrip2, CRGB::Black);

    // Desenha os LEDs da corrente
    for (int i = 0; i < _currentNumberOfChainLeds; i++)
    {
        int index1 = (_chainPosition + i) % _numLedsStrip1;
        int index2 = (_chainPosition + i) % _numLedsStrip2;

        _strip1Leds[index1] = CRGB(_currentColor);
        _strip2Leds[index2] = CRGB(_currentColor);
    }

    FastLED.show();

    // Avança a posição
    _chainPosition++;

    if (_chainPosition >= max(_numLedsStrip1, _numLedsStrip2))
    {
        _chainPosition = 0;
    }

    delay(_currentDelay);
}


/*
void LEDs::chainingEffect()
{
    for (int i = 0; i < _numLedsStrip1; i++)
    {
        _strip1Leds[i] = CRGB::Black;
        _strip2Leds[i] = CRGB::Black;
    }
    FastLED.show();
    for(int i = 0; i < _currentNumberOfChainLeds; i++) {
        if (_chainPosition + i < _numLedsStrip1)
        {
            _strip1Leds[_chainPosition + i] = CRGB(_currentColor);
            _strip2Leds[_chainPosition + i] = CRGB(_currentColor);
        } else {
            break;
        }
    }

    FastLED.show();
    _chainPosition++;
    if (_chainPosition >= max(_numLedsStrip1, _numLedsStrip2))
    {
        _chainPosition = 0;
    }
    delay(_currentDelay);
}*/

void LEDs::turnOff()
{
    for (int i = 0; i < _numLedsStrip1; i++)
    {
        _strip1Leds[i] = CRGB::Black;
    }
    for (int i = 0; i < _numLedsStrip2; i++)
    {
        _strip2Leds[i] = CRGB::Black;
    }
    FastLED.show();
}

void LEDs::blinkColor()
{
    static bool state = false;

    if (state)
    {
        for (int i = 0; i < _numLedsStrip1; i++)
        {
            _strip1Leds[i] = CRGB(_currentColor);
        }
        for (int i = 0; i < _numLedsStrip2; i++)
        {
            _strip2Leds[i] = CRGB(_currentColor);
        }
    }
    else
    {
        for (int i = 0; i < _numLedsStrip1; i++)
        {
            _strip1Leds[i] = CRGB::Black;
        }
        for (int i = 0; i < _numLedsStrip2; i++)
        {
            _strip2Leds[i] = CRGB::Black;
        }
    }

    FastLED.show();
    state = !state;
    delay(_currentDelay);
}

void LEDs::staticColor()
{
    for (int i = 0; i < _numLedsStrip1; i += 3)
    {
        for(int j = 0; j < 3; j++) {
            if(i + j < _numLedsStrip1) {
                _strip1Leds[i + j] = CRGB(_currentColor);
            }
        }
    }
    FastLED.show();
    for (int i = 0; i < _numLedsStrip2; i += 3)
    {
        for(int j = 0; j < 3; j++) {
            if(i + j < _numLedsStrip2) {
                _strip2Leds[i + j] = CRGB(_currentColor);
            }
        }
    }
    FastLED.show();
}