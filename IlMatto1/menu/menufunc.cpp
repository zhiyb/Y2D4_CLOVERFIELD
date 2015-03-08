#include <avr/io.h>
#include <avr/pgmspace.h>
#include "menu.h"
#include "pool.h"
#include "common.h"

namespace menu
{
	namespace sketch
	{
		void sketchMode(bool shared);
	}

	namespace diagnosis
	{
		static void packageTest(const char *name, uint8_t command, uint8_t length = 0, const uint8_t *data = 0);
	}
}

using namespace menu;

bool menu::toggle::func(bool enter)
{
	PINB |= _BV(7);
	return false;
}

void menu::diagnosis::packageTest(const char *name, uint8_t command, uint8_t length, const uint8_t *data)
{
	tft.vsNormal();
	tft.setForeground(0x667F);
	tft.setBackground(Black);
	tft.setZoom(1);
	tft.clean();

	puts_P(PSTR("*** Il Matto1 testing ***"));
	struct package_t *pkg;
	if (!command)
		goto pool;

	puts_P(PSTR("Waiting for other commands..."));
	while (!(pkg = uart0_txPackage()));
	puts_P(name);
	pkg->command = command;
	pkg->length = length;
	for (uint8_t i = 0; i < length; i++)
		pkg->data[i] = *(data + i);
	pkg->valid++;
	uart0_send();

pool:
	pkg = uart0_rxPackage();
	if (pkg) {
		tft.setForeground(Green);
		printf("Package received: ");
		printf("C/%u", pkg->command);
		if (pkg->command & COM_DATA) {
			printf(", L/%u, \n", pkg->length);
			uint8_t *ptr = pkg->data;
			for (uint8_t i = 0; i < pkg->length; i++)
				printf("%u, ", *ptr++);
		}
		putchar('\n');
		uart0_done(pkg);
	}

	if (uart0_ack()) {
		tft.setForeground(Yellow);
		puts("ACK received.");
	}

	if (touch.pressed()) {
		//uart0_reset();
		return;
	}
	goto pool;
}

bool menu::diagnosis::reset::func(bool enter)
{
	PINB |= _BV(7);
	uart0_reset();
	return false;
}

bool menu::diagnosis::pool::func(bool enter)
{
	packageTest(0, 0);
	return false;
}

bool menu::diagnosis::ping::func(bool enter)
{
	packageTest(PSTR("PING for Il Matto 2..."), COM_PING);
	return false;
}

bool menu::diagnosis::wakeup::func(bool enter)
{
	packageTest(PSTR("Wakeup wireless module..."), COM_WAKEUP);
	return false;
}

bool menu::diagnosis::suspend::func(bool enter)
{
	packageTest(PSTR("Suspend wireless module..."), COM_SUSPEND);
	return false;
}

bool menu::diagnosis::w_ping::func(bool enter)
{
	packageTest(PSTR("Ping other end..."), COM_W_PING);
	return false;
}

bool menu::diagnosis::w_sound::func(bool enter)
{
	packageTest(PSTR("Enabling sound transceiver..."), COM_W_SOUND);
	return false;
}

bool menu::diagnosis::w_sound_end::func(bool enter)
{
	packageTest(PSTR("Disabling sound transceiver..."), COM_W_SOUND_END);
	return false;
}

bool menu::diagnosis::w_send::func(bool enter)
{
	uint8_t data[BUFFER_SIZE];
	for (uint8_t i = 0; i < BUFFER_SIZE; i++)
		data[i] = i;
	packageTest(PSTR("Sending test data..."), COM_W_SEND, BUFFER_SIZE, data);
	return false;
}

bool menu::sketch::single::func(bool enter)
{
	sketchMode(false);
	return false;
}

bool menu::sketch::shared::func(bool enter)
{
	sketchMode(true);
	return false;
}

bool menu::settings::calibration::func(bool enter)
{
	tft.vsNormal();
	touch.recalibrate();
	return false;
}

bool menu::settings::keypadcal::func(bool enter)
{
	tft.vsNormal();
	keypad.recalibrate();
	return false;
}

// Pooling functions

void menu::sketch::sketchMode(bool shared)
{
	tft.vsNormal();
	::sketch.init();
	::sketch.setShared(shared);

	for (;;) {
		uart0_done(::sketch.pool(status.pool(pool::pool())));

		if (shared) {
			status.checkRemote();
			indicator::checkRemote(false);
		} else {
			status.checkIlMatto2();
			indicator::checkIlMatto2(false);
		}

		if (touch.pressed()) {
			rTouch::coord_t pos = touch.position();
			if (keypad.outsideLeft(pos.x + 10))
				break;
		}
	}
}

bool menu::diagnosis::keypad::func(bool enter)
{
	tft.vsNormal();
	::keypad.display();

	for (;;) {
		uart0_done(status.pool(::pool::pool()));
		status.checkIlMatto2();
		indicator::checkIlMatto2(true);
		if (!::keypad.testPool())
			break;
	}

	return false;
}
