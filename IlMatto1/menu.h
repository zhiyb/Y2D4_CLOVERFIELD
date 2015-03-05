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

	namespace toggle {bool func(void);}

	namespace diagnosis
	{
		namespace ping {bool func(void);}
		namespace wakeup {bool func(void);}
		namespace suspend {bool func(void);}

		namespace w_ping {bool func(void);}
		namespace w_sound {bool func(void);}
		namespace w_sound_end {bool func(void);}
		namespace w_send {bool func(void);}
	}

	namespace sketch {bool func(void);}

	namespace root
	{
		extern listItem item;
	}
}

#endif
