#ifndef __SUB_SYS__
#define __SUB_SYS__

#include "Config.h"
#include "components/button/Button.h"
#include "src/components/servo/ServoMImpl.h"
#include "src/components/distantiometer/Sonar.h"
#include "components/lcd/Lcd.h"
#include "components/lcd/LcdI2C.h"
#include "components/button/ButtonImpl.h"

class SubSys {
    private:
        DistanceSensor *distanceSensor;
        ServoM *servoMotor;
        Lcd *lcd;
        Button *button;

    public:
        SubSys();
        int getDistance();
        bool isButtonPressed();
        ServoM *getServoMotor();
        Lcd *getLcd();
};

#endif
