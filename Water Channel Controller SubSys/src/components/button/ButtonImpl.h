#ifndef __BUTTONIMPL__
#define __BUTTONIMPL__

#include "Button.h"

class ButtonImpl : public Button {
	public: 
		ButtonImpl(unsigned short pin);
		bool isClicked();
};

#endif
