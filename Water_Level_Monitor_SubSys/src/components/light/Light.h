#ifndef __LIGHT__
#define __LIGHT__

#include "../Component.h"

class Light : public Component {
    public:
        Light(unsigned short pin);
        virtual void switchLight(bool state) = 0;
        virtual bool isOn() = 0;
};

#endif
