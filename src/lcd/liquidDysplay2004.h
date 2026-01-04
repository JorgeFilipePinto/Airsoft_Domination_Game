#pragma once
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include "messageStruct.h"


class LiquidDysplay2004 {
    public:
        void init();
        void clear();
        void print(MessageStruct* messages, uint8_t count);
        void printAt(String message, uint8_t x, uint8_t y);
        void clearLine(uint8_t line);
        void initLoadingChars();
        void drawLoadingBar(uint8_t position, uint8_t stage);

    private:
        LiquidCrystal_I2C* lcd;
        uint8_t I2C_ADDR = 0x27;
        uint8_t BACKLIGHT_PIN = 3;
        uint8_t En_pin = 2;
        uint8_t Rw_pin = 1;
        uint8_t Rs_pin = 0;
        uint8_t D4_pin = 4;
        uint8_t D5_pin = 5;
        uint8_t D6_pin = 6;
        uint8_t D7_pin = 7;
        void test();
};