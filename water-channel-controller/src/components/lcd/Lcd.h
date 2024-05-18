#ifndef __LCD__
#define __LCD__

#include "components/Component.h"

class Lcd : public Component {
    public:
        Lcd();
        Lcd(Pin pins[], int numPins);
        virtual void displayText(const char* text) = 0;
        virtual void newLine() = 0;
        virtual void clearScreen() = 0;
        virtual void setPosition(int row, int col) = 0;
};

#endif
