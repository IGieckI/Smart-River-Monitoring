#ifndef LCDI2C_H
#define LCDI2C_H

#include <LiquidCrystal_I2C.h>
#include "Lcd.h"

class LcdI2C : public Lcd {
    private:
        LcdI2C(Pin pins[], int numPins);

    protected:
        LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);
        int row = 0, col = 0;
    
    public:
        LcdI2C(unsigned short I2CAddress = 0x27, int columns = 16, int rows = 2, unsigned short sdaPin = A4, unsigned short sclPin = A5);
        virtual void displayText(const char* text);
        virtual void newLine();
        virtual void clearScreen();
        virtual void setPosition(int row, int col);
};

#endif
