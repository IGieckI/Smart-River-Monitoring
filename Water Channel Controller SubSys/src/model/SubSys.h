#ifndef __SUB_SYS__
#define __SUB_SYS__

#include "Config.h"
#include "components/button/Button.h"
#include "components/servo/ServoMImpl.h"
#include "components/lcd/Lcd.h"
#include "components/lcd/LcdI2C.h"
#include "components/button/ButtonImpl.h"
#include "components/potentiometer/Potentiometer.h"
#include "components/potentiometer/PotentiometerImpl.h"
#include "components/light/Led.h"

class SubSys {
    private:
        ServoM *servoMotor;
        LcdI2C *lcd;
        ButtonImpl *button;
        PotentiometerImpl *potentiometer;
        bool manuelMode;
        uint8_t valveOpening;
        Led *greenLed;
        Led *redLed;
    public:
        SubSys();
        int getWaterDistance();
        bool isButtonPressed();
        ServoM *getServoMotor();
        LcdI2C *getLcd();
        void changeMode();
        bool isManuelMode();
        PotentiometerImpl *getPotentiometer();
        void setValveOpening(uint8_t val);
        uint8_t getValveOpening();
        Led *getGreenLed();
        Led *getRedLed();
};

#endif
