#ifndef SERVOIMPL_H
#define SERVOIMPL_H

const int CLOSE_GATE_DEGREE = 180;
const int OPEN_GATE_DEGREE = 70;

#include "ServoM.h"
#include <ServoTimer2.h>

class ServoMImpl : public ServoM {
    private:
        ServoTimer2 servo;
        int angle;
    public:
        ServoMImpl(unsigned short pin);
        void setPosition(int _angle);
        int getPosition();
        bool isClose();
        bool isOpen();
        int getDefaultCloseAngle();
        int getDefaultOpenAngle();
};

#endif
