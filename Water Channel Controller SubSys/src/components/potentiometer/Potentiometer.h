#ifndef _POTENTIMETER_H_
#define _POTENTIMETER_H_

#include "components/Component.h"

class Potentiometer : public Component {
    public:
        Potentiometer(unsigned short pin);
        virtual int getValue() = 0;

};

#endif