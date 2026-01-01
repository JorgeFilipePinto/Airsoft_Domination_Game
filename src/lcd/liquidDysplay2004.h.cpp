#include "liquidDysplay2004.h"

void LiquidDysplay2004::init()
{
    lcd = new LiquidCrystal_I2C(I2C_ADDR, En_pin, Rw_pin, Rs_pin, D4_pin, D5_pin, D6_pin, D7_pin);
    lcd->begin(20, 4);
    lcd->setBacklightPin(BACKLIGHT_PIN, POSITIVE);
    lcd->setBacklight(HIGH);
    lcd->home();
    initLoadingChars();
}

void LiquidDysplay2004::print(MessageStruct *messages, uint8_t count)
{
    for (int i = 0; i < count; i++)
    {
        lcd->setCursor(messages[i].x, messages[i].y);
        lcd->print(messages[i].message);
    }
}

void LiquidDysplay2004::printAt(String message, uint8_t x, uint8_t y)
{
    lcd->setCursor(x, y);
    lcd->print(message);
}


void LiquidDysplay2004::clearLine(uint8_t line)
{
    lcd->setCursor(0, line);
    for(int i = 0; i < 20; i++)
    {
        lcd->print(" ");
    }
}


void LiquidDysplay2004::initLoadingChars()
{
    // Criar caracteres personalizados UMA VEZ
    byte empty[] = {B00000, B00000, B00000, B00000, B00000, B00000, B00000, B00000};
    byte p25[] = {B10000, B10000, B10000, B10000, B10000, B10000, B10000, B10000};
    byte p50[] = {B11000, B11000, B11000, B11000, B11000, B11000, B11000, B11000};
    byte p75[] = {B11100, B11100, B11100, B11100, B11100, B11100, B11100, B11100};
    byte full[] = {B11111, B11111, B11111, B11111, B11111, B11111, B11111, B11111};

    lcd->createChar(1, empty);
    lcd->createChar(2, p25);
    lcd->createChar(3, p50);
    lcd->createChar(4, p75);
    lcd->createChar(5, full);
}


void LiquidDysplay2004::drawLoadingBar(uint8_t position, uint8_t stage)
{
    lcd->setCursor(position, 2);
    lcd->write(byte(stage));
}

void LiquidDysplay2004::clear()
{
    lcd->clear();
    lcd->home();
}

void LiquidDysplay2004::test()
{
    lcd->print("LiquidDysplay2004");
    lcd->setCursor(0, 1);
    lcd->print("Display 20x4 LCD");
    lcd->setCursor(0, 2);
    lcd->print("Test & demonstration");
    lcd->setCursor(0, 3);
    lcd->print("Welcome!");
}