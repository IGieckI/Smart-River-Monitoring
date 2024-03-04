#ifndef __SUB_SYS__
#define __SUB_SYS__

#include "Config.h"
#include "components/button/Button.h"
#include "components/servo/ServoMImpl.h"
#include "components/distantiometer/Sonar.h"
#include "components/lcd/Lcd.h"
#include "components/lcd/LcdI2C.h"
#include "components/button/ButtonImpl.h"
#include "components/distantiometer/DistanceSensor.h"
#include "components/msg/MsgService.h"

class SubSys {
    private:
        DistanceSensor *distanceSensor;
        ServoM *servoMotor;
        Lcd *lcd;
        Button *button;
        bool manuelMode;
    public:
        SubSys();
        int getWaterDistance();
        bool isButtonPressed();
        ServoM *getServoMotor();
        Lcd *getLcd();
        void changeMode();
        bool isManuelMode();
};

#endif
