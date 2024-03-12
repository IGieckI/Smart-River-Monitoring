#ifndef __BUTTONIMPL__
#define __BUTTONIMPL__

#include "Button.h"

class ButtonImpl : public Button {
	private:
		int lastClickTime;
	public: 
		ButtonImpl(unsigned short pin);
		bool isClicked();
};

#endif
