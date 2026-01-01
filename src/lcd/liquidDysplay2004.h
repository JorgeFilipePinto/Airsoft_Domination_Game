#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include "messageStruct.h"
#pragma once



class LiquidDysplay2004 {
    public:
        void init();
        void clear();
        void print(MessageStruct* messages, uint8_t count);

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