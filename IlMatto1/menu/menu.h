/*
 *  Author: Yubo Zhi (yz39g13@soton.ac.uk)
 */

#ifndef MENU_H
#define MENU_H

#include <list.h>
#include <tft.h>
#include <rtouch.h>
#include "sketch.h"

namespace menu
{
	namespace toggle {bool func(bool enter);}

	namespace lock {bool func(bool enter);}

	namespace audio {bool func(bool enter);}

	namespace diagnosis
	{
		namespace reset {bool func(bool enter);}
		namespace pool_quiet {bool func(bool enter);}
		namespace pool {bool func(bool enter);}

		namespace ping {bool func(bool enter);}
		namespace wakeup {bool func(bool enter);}
		namespace suspend {bool func(bool enter);}

		namespace w_ping {bool func(bool enter);}
		namespace w_audio_tx {bool func(bool enter);}
		namespace w_audio_stop {bool func(bool enter);}
		namespace w_send {bool func(bool enter);}
		namespace w_data {bool func(bool enter);}

		namespace keypad {bool func(bool enter);}
		namespace text {bool func(bool enter);}
	}

	namespace sketch
	{
		namespace single {bool func(bool enter);}
		namespace shared {bool func(bool enter);}
	}

	namespace game
	{
		namespace tictactoe {bool func(bool enter);}
	}

	namespace settings
	{
		namespace frequency {bool func(bool enter);}
		namespace reset_pin {bool func(bool enter);}
		namespace calibration {bool func(bool enter);}
		namespace keypadcal {bool func(bool enter);}
	}

	namespace root
	{
		extern listItem item;
	}
}

#endif
