/*
 * Author: Yubo Zhi (yz39g13@soton.ac.uk)
 */

#include <avr/io.h>
#include <avr/pgmspace.h>
#include "menu.h"

namespace menu
{

/****************** Miscellaneous item ******************/
namespace misc
{
	static const uint8_t PROGMEM icon_test[] = {
		0x83,0xC1,0x40,0x02,0x20,0x04,0x13,0xC8,0x0C,0x30,0x0C,0x30,0x92,0x49,0x91,0x89,
		0x91,0x89,0x92,0x49,0x0C,0x30,0x0C,0x30,0x13,0xC8,0x20,0x04,0x40,0x02,0x83,0xC1,
	};
}

#ifdef DEBUG
namespace toggle
{
	static const char PROGMEM name[] = "LED Toggle";
	static listItem item = {name, misc::icon_test, 0, func};
}
#endif

namespace lock
{
	static const char PROGMEM name[] = "Lock";
	static const uint8_t PROGMEM icon[] = {
		0x00,0x00,0x00,0x00,0x03,0xC0,0x04,0x20,0x08,0x10,0x08,0x10,0x08,0x10,0x3F,0xFC,
		0x22,0xB4,0x2A,0x94,0x22,0xA4,0x2E,0xB4,0x3F,0xFC,0x3F,0xFC,0x3F,0xFC,0x3F,0xFC,
	};
	static listItem item = {name, icon, 0, func};
}

namespace audio
{
	static const char PROGMEM name[] = "Audio";
	static const uint8_t PROGMEM icon[] = {
		0x01,0x00,0x03,0x10,0x05,0x08,0x09,0x24,0x11,0x12,0xE1,0x4A,0xA1,0x2A,0xA1,0x2A,
		0xA1,0x2A,0xA1,0x2A,0xE1,0x4A,0x11,0x12,0x09,0x24,0x05,0x08,0x03,0x10,0x01,0x00,
	};
	static listItem item = {name, icon, 0, func};
}

namespace game
{
	static const char PROGMEM name[] = "Game";
	static const uint8_t PROGMEM icon[] = {
		0x38,0x41,0x44,0x22,0x82,0x14,0x82,0x08,0x82,0x14,0x44,0x22,0x78,0x41,0x00,0x00,
		0x00,0x00,0x10,0x7F,0x28,0x41,0x28,0x41,0x44,0x41,0x44,0x41,0x82,0x41,0xFE,0x7F,
	};

	namespace tictactoe
	{
		static const char PROGMEM name[] = "Tic Tac Toe";
		static listItem item = {name, icon, 0, func};
	}

	static const listItem *items[] = {&tictactoe::item, 0};
	static listItem item = {name, icon, items, 0};
}

#ifdef DEBUG
namespace diagnosis
{
	namespace reset
	{
		static const char PROGMEM name[] = "UART Reset";
		static listItem item = {name, misc::icon_test, 0, func};
	}

	namespace pool_quiet
	{
		static const char PROGMEM name[] = "Quiet pooling";
		static listItem item = {name, misc::icon_test, 0, func};
	}

	namespace pool
	{
		static const char PROGMEM name[] = "Pooling";
		static listItem item = {name, misc::icon_test, 0, func};
	}

	namespace ping
	{
		static const char PROGMEM name[] = "Ping";
		static listItem item = {name, misc::icon_test, 0, func};
	}

	namespace wakeup
	{
		static const char PROGMEM name[] = "Wake up";
		static listItem item = {name, misc::icon_test, 0, func};
	}

	namespace suspend
	{
		static const char PROGMEM name[] = "Suspend";
		static listItem item = {name, misc::icon_test, 0, func};
	}

	namespace w_ping
	{
		static const char PROGMEM name[] = "W: Ping";
		static listItem item = {name, misc::icon_test, 0, func};
	}

	namespace w_audio_tx
	{
		static const char PROGMEM name[] = "W: Audio TX";
		static listItem item = {name, misc::icon_test, 0, func};
	}

	namespace w_audio_stop
	{
		static const char PROGMEM name[] = "W: Audio stop";
		static listItem item = {name, misc::icon_test, 0, func};
	}

	namespace w_send
	{
		static const char PROGMEM name[] = "W: Test data";
		static listItem item = {name, misc::icon_test, 0, func};
	}

	namespace w_data
	{
		static const char PROGMEM name[] = "W: Lots of data";
		static listItem item = {name, misc::icon_test, 0, func};
	}

	namespace keypad
	{
		static const char PROGMEM name[] = "Test keypad";
		static listItem item = {name, misc::icon_test, 0, func};
	}

	namespace text
	{
		static const char PROGMEM name[] = "Text input";
		static listItem item = {name, misc::icon_test, 0, func};
	}

	static const char PROGMEM name[] = "Diagnosis";
	static const uint8_t PROGMEM icon[] = {
		0x80,0x01,0x43,0xC2,0x24,0x24,0x08,0x10,0x10,0x08,0x10,0x08,0xD1,0x8B,0x12,0x48,
		0x0A,0x50,0x06,0x60,0x23,0xC4,0x42,0x42,0x83,0xC1,0x02,0x40,0x03,0xC0,0x01,0x80,
	};
	using namespace misc;
	static const listItem *items[] = {
		&game::item,
		&reset::item, &pool_quiet::item, &pool::item,
		&ping::item, &wakeup::item, &suspend::item,
		&w_ping::item, &w_audio_tx::item, &w_audio_stop::item,
		&w_send::item, &w_data::item,
		&keypad::item, &text::item, &toggle::item, 0
	};
	static listItem item = {name, icon, items, 0};
}
#endif

namespace sketch
{
	static const char PROGMEM name[] = "Sketch";
	static const uint8_t PROGMEM icon[] = {
		0x02,0x03,0x05,0x06,0x08,0x8C,0x10,0x58,0x20,0x30,0x44,0x70,0x84,0xC8,0xC7,0x84,
		0xA0,0x82,0x50,0xE1,0x28,0x22,0x14,0x05,0x0A,0x0A,0x05,0x14,0x02,0xA8,0x01,0x50,
	};

	namespace single
	{
		static const char PROGMEM name[] = "Free play";
		static listItem item = {name, icon, 0, func};
	}

	namespace shared
	{
		static const char PROGMEM name[] = "Shared";
		static listItem item = {name, icon, 0, func};
	}

	static const listItem *items[] = {&single::item, &shared::item, 0};
	static listItem item = {name, icon, items, 0};
}

namespace text
{
	static const char PROGMEM name[] = "Text";
	static const uint8_t PROGMEM icon[] = {
		0x3F,0xFC,0x40,0x02,0x89,0x21,0x89,0x01,0x8F,0x61,0x89,0x21,0x89,0x21,0x89,0x71,
		0x80,0x01,0x40,0x02,0x3F,0xC4,0x00,0x22,0x00,0x12,0x00,0x09,0x00,0x05,0x00,0x02,
	};
	static listItem item = {name, icon, 0, func};
}

namespace settings
{
	namespace frequency
	{
		static const char PROGMEM name[] = "Frequency";
		static listItem item = {name, misc::icon_test, 0, func};
	}

	namespace reset_pin
	{
		static const char PROGMEM name[] = "Reset PIN";
		static listItem item = {name, misc::icon_test, 0, func};
	}

	namespace calibration
	{
		static const char PROGMEM name[] = "Calibration";
		static listItem item = {name, misc::icon_test, 0, func};
	}

	namespace keypadcal
	{
		static const char PROGMEM name[] = "Keypad calib";
		static listItem item = {name, misc::icon_test, 0, func};
	}

	static const char PROGMEM name[] = "Settings";
	static const uint8_t PROGMEM icon[] = {
		0x03,0xC0,0x12,0x48,0x2A,0x54,0x46,0x62,0x20,0x04,0x10,0x08,0xF1,0x8F,0x82,0x41,
		0x82,0x41,0xF1,0x8F,0x10,0x08,0x20,0x04,0x46,0x62,0x2A,0x54,0x12,0x48,0x03,0xC0,
	};
	static const listItem *items[] = {&frequency::item, &reset_pin::item, &calibration::item, &keypadcal::item, 0};
	static listItem item = {name, icon, items, 0};
}

namespace help
{
	static const char PROGMEM name[] = "Help";
	static const uint8_t PROGMEM icon[] = {
		0x07,0xE0,0x0F,0xF0,0x1C,0x38,0x18,0x18,0x18,0x18,0x00,0x38,0x00,0x70,0x00,0xE0,
		0x01,0xC0,0x01,0x80,0x01,0x80,0x01,0x80,0x00,0x00,0x00,0x00,0x01,0x80,0x01,0x80,
	};
	static const listItem *items[] = {0};
	static listItem item = {name, icon, items, 0};
}

namespace root
{
	static const char PROGMEM name[] = "Talkie walkie 2k";
	static const uint8_t PROGMEM icon[] = {
		0x00,0x00,0x07,0xE0,0x0F,0xF0,0x1F,0xF8,0x3F,0xFC,0x7F,0xFE,0x7E,0x7E,0x7C,0x3E,
		0x7C,0x3E,0x7E,0x7E,0x7F,0xFE,0x3F,0xFC,0x1F,0xF8,0x0F,0xF0,0x07,0xE0,0x00,0x00,
	};
	static const listItem *items[] = {&lock::item, &audio::item, &sketch::item, &text::item, &game::item, &settings::item, &help::item,
#ifdef DEBUG
		&diagnosis::item,
#endif
		0};
	listItem item = {name, icon, items, 0};
}

}
