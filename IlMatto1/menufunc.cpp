#include <avr/io.h>
#include <avr/pgmspace.h>
#include <colours.h>
#include "menu.h"
#include "uart0.h"
#include "sketch.h"

namespace menu
{
	static tft_t *tft;
	static rTouch *touch;
	static sketch_t *skt;

	namespace diagnosis
	{
		static void packageTest(const char *name, uint8_t command);
	}
}

using namespace menu;
using namespace colours::b16;

void menu::setTFT(tft_t *ptr)
{
	tft = ptr;
}

void menu::setTouch(rTouch *ptr)
{
	touch = ptr;
}

void menu::setSketch(sketch_t *ptr)
{
	skt = ptr;
}

bool menu::toggle::func(void)
{
	PINB |= _BV(7);
	return false;
}

void menu::diagnosis::packageTest(const char *name, uint8_t command)
{
	tft->vsNormal();
	tft->setForeground(0x667F);
	tft->setBackground(Black);
	tft->setZoom(1);
	tft->clean();

	puts_P(PSTR("*** Il Matto1 testing ***"));
	struct package_t *pkg;

	puts_P(PSTR("Request for other commands..."));
	while (!(pkg = uart0_txPackage()));
	puts_P(name);
	pkg->command = command;
	pkg->length = 0;
	pkg->valid++;
	uart0_send();

	for (uint8_t i = 0; i < 50 && uart0_txStatus() != UART0_TX_IDLE; i++)
		_delay_ms(100);

	if (uart0_txStatus() & UART0_TX_END) {
		tft->setForeground(Yellow);
		puts("End of data response received!");
	} else if (uart0_txStatus() == UART0_TX_IDLE) {
		tft->setForeground(Green);
		puts("Valid response received!");
	} else {
		tft->setForeground(Red);
		puts("Response receive timeout!");
	}

	touch->waitForPress();
}

bool menu::diagnosis::ping::func(void)
{
	packageTest(PSTR("PING for Il Matto 2..."), COM_PING);
	return false;
}

bool menu::diagnosis::wakeup::func(void)
{
	packageTest(PSTR("Wakeup wireless module..."), COM_WAKEUP);
	return false;
}

bool menu::diagnosis::suspend::func(void)
{
	packageTest(PSTR("Suspend wireless module..."), COM_SUSPEND);
	return false;
}

bool menu::diagnosis::w_ping::func(void)
{
	packageTest(PSTR("Ping other end..."), COM_W_PING);
	return false;
}

bool menu::sketch::func(void)
{
	skt->init();

	for (;;)
		if (!skt->pool())
			break;

	return false;
}
