#include "components/lcd/LcdI2C.h"

LcdI2C::LcdI2C(unsigned short I2CAddress, int columns, int rows, unsigned short sdaPin, unsigned short sclPin) : Lcd() {
    this->lcd = LiquidCrystal_I2C(I2CAddress, columns, rows);
    this->lcd.init();
    this->lcd.backlight();

    Pin pins[2] = { Pin(sdaPin, OUTPUT), Pin(sclPin, OUTPUT) };
    LcdI2C(pins, 2);
}

LcdI2C::LcdI2C(Pin pins[], int numPins) : Lcd(pins, numPins) { }

void LcdI2C::displayText(const char* text) {
    this->lcd.setCursor(col, row);
    lcd.print(text);
}

void LcdI2C::newLine() {
    this->lcd.setCursor(0, ++col);
}

void LcdI2C::clearScreen() {
    lcd.clear();
    col = 0;
    row = 0;
}