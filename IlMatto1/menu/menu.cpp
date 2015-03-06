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
	static const char PROGMEM name_03[] = "Item 3";
	static listItem item_03 = {name_03, icon_test, 0, 0};
	static const char PROGMEM name_04[] = "Item 4";
	static listItem item_04 = {name_04, icon_test, 0, 0};
}

namespace toggle
{
	static const char PROGMEM name[] = "LED Toggle";
	static listItem item = {name, misc::icon_test, 0, func};
}

namespace diagnosis
{
	namespace reset
	{
		static const char PROGMEM name[] = "UART Reset";
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

	namespace w_sound
	{
		static const char PROGMEM name[] = "W: Sound";
		static listItem item = {name, misc::icon_test, 0, func};
	}

	namespace w_sound_end
	{
		static const char PROGMEM name[] = "W: Sound end";
		static listItem item = {name, misc::icon_test, 0, func};
	}

	namespace w_send
	{
		static const char PROGMEM name[] = "W: Send test data";
		static listItem item = {name, misc::icon_test, 0, func};
	}

	static const char PROGMEM name[] = "Diagnosis";
	static const uint8_t PROGMEM icon[] = {
		0x80,0x01,0x43,0xC2,0x24,0x24,0x08,0x10,0x10,0x08,0x10,0x08,0xD1,0x8B,0x12,0x48,
		0x0A,0x50,0x06,0x60,0x23,0xC4,0x42,0x42,0x83,0xC1,0x02,0x40,0x03,0xC0,0x01,0x80,
	};
	using namespace misc;
	static const listItem *items[] = {
		&reset::item, &pool::item,
		&ping::item, &wakeup::item, &suspend::item,
		&w_ping::item, &w_sound::item, &w_sound_end::item,
		&w_send::item,
		&toggle::item, &item_03, &item_04, 0
	};
	static listItem item = {name, icon, items, 0};
}

namespace sketch
{
	static const char PROGMEM name[] = "Sketch";
	static const uint8_t PROGMEM icon[] = {
		0x02,0x03,0x05,0x06,0x08,0x8C,0x10,0x58,0x20,0x30,0x44,0x70,0x84,0xC8,0xC7,0x84,
		0xA0,0x82,0x50,0xE1,0x28,0x22,0x14,0x05,0x0A,0x0A,0x05,0x14,0x02,0xA8,0x01,0x50,
	};
	listItem item = {name, icon, 0, func};
}

namespace game
{
	static const char PROGMEM name[] = "Game";
	static const uint8_t PROGMEM icon[] = {
		0x38,0x41,0x44,0x22,0x82,0x14,0x82,0x08,0x82,0x14,0x44,0x22,0x78,0x41,0x00,0x00,
		0x00,0x00,0x10,0x7F,0x28,0x41,0x28,0x41,0x44,0x41,0x44,0x41,0x82,0x41,0xFE,0x7F,
	};

	namespace tetris
	{
		static const char PROGMEM name[] = "Tetris";
		static listItem item = {name, icon, 0, 0};
	}

	static const listItem *items[] = {&tetris::item, 0};
	listItem item = {name, icon, items, 0};
}

namespace root
{
	static const char PROGMEM name[] = "Root";
	static const uint8_t PROGMEM icon[] = {
		0x00,0x00,0x07,0xE0,0x0F,0xF0,0x1F,0xF8,0x3F,0xFC,0x7F,0xFE,0x7E,0x7E,0x7C,0x3E,
		0x7C,0x3E,0x7E,0x7E,0x7F,0xFE,0x3F,0xFC,0x1F,0xF8,0x0F,0xF0,0x07,0xE0,0x00,0x00,
	};
	static const listItem *items[] = {&diagnosis::item, &sketch::item, &game::item, &toggle::item, 0};
	listItem item = {name, icon, items, 0};
}

}
