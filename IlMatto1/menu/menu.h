#ifndef MENU_H
#define MENU_H

#include <list.h>
#include <tft.h>
#include <rtouch.h>
#include "sketch.h"

namespace menu
{
	void setTFT(tft_t *ptr);
	void setTouch(rTouch *ptr);
	void setSketch(sketch_t *ptr);

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
	}

	namespace sketch {bool func(bool enter);}

	namespace root
	{
		extern listItem item;
	}
}

#endif
