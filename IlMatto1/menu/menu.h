#ifndef MENU_H
#define MENU_H

#include <list.h>
#include <tft.h>
#include <rtouch.h>
#include "sketch.h"

namespace menu
{
	namespace toggle {bool func(bool enter);}

	namespace diagnosis
	{
		namespace reset {bool func(bool enter);}
		namespace pool {bool func(bool enter);}

		namespace ping {bool func(bool enter);}
		namespace wakeup {bool func(bool enter);}
		namespace suspend {bool func(bool enter);}

		namespace w_ping {bool func(bool enter);}
		namespace w_sound {bool func(bool enter);}
		namespace w_sound_end {bool func(bool enter);}
		namespace w_send {bool func(bool enter);}

		namespace keypad {bool func(bool enter);}
	}

	namespace sketch {bool func(bool enter);}

	namespace settings
	{
		namespace calibration {bool func(bool enter);}
		namespace keypadcal {bool func(bool enter);}
	}

	namespace root
	{
		extern listItem item;
	}
}

#endif
