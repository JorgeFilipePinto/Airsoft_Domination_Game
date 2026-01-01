#include "liquidDysplay2004.h"


void LiquidDysplay2004::init() {
    lcd = new LiquidCrystal_I2C	(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);
    lcd->begin(20, 4);
    lcd->setBacklightPin(BACKLIGHT_PIN, POSITIVE);
    lcd->setBacklight(HIGH);
    lcd->home();
}


void LiquidDysplay2004::print(MessageStruct* messages, uint8_t count) {
    for(int i = 0; i < count; i++) {
        lcd->setCursor(messages[i].x, messages[i].y);
        lcd->print(messages[i].message);
    }
}


void LiquidDysplay2004::clear() {
    lcd->home();
}


void LiquidDysplay2004::test() {
  lcd->print("LiquidDysplay2004");  
  lcd->setCursor ( 0, 1 );
  lcd->print("Display 20x4 LCD");
  lcd->setCursor ( 0, 2 );
  lcd->print("Test & demonstration");
  lcd->setCursor ( 0, 3 );
  lcd->print("Welcome!");
}