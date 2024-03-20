#ifndef __BUTTON__
#define __BUTTON__

#include "../Component.h"

class Button : public Component {
    public:
        Button(unsigned short pin);
        virtual bool isClicked() = 0;
};

#endif
