#ifndef PIN_H
#define PIN_H

class Pin {
    private:
        unsigned short pinNumber;
        unsigned short type;
    
    public:
        Pin(unsigned short pinNumber, unsigned short type);
        Pin();
        int getPinNumber();
        unsigned short getPinType();
};

#endif
