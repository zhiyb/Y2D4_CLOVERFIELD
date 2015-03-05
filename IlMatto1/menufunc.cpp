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
		static void packageTest(const char *name, uint8_t command, uint8_t length = 0, const uint8_t *data = 0);
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

void menu::diagnosis::packageTest(const char *name, uint8_t command, uint8_t length, const uint8_t *data)
{
	tft->vsNormal();
	tft->setForeground(0x667F);
	tft->setBackground(Black);
	tft->setZoom(1);
	tft->clean();

	puts_P(PSTR("*** Il Matto1 testing ***"));
	struct package_t *pkg;

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
		tft->setForeground(Green);
		printf("Package received: ");
		printf("C/%u", pkg->command);
		if (pkg->command & COM_DATA) {
			printf(", L/%u, \n", pkg->length);
			uint8_t *ptr = pkg->data;
			for (uint8_t i = 0; i < pkg->length; i++)
				printf("%u, ", *ptr++);
		}
		putchar('\n');
		pkg->valid = 0;
		uart0_received();
	}

	if (uart0_ack()) {
		tft->setForeground(Yellow);
		puts("ACK received.");
	}

	if (touch->pressed()) {
		//uart0_reset();
		return;
	}
	goto pool;
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

bool menu::diagnosis::w_sound::func(void)
{
	packageTest(PSTR("Enabling sound transceiver..."), COM_W_SOUND);
	return false;
}

bool menu::diagnosis::w_sound_end::func(void)
{
	packageTest(PSTR("Disabling sound transceiver..."), COM_W_SOUND_END);
	return false;
}

bool menu::diagnosis::w_send::func(void)
{
	uint8_t data[BUFFER_SIZE];
	for (uint8_t i = 0; i < BUFFER_SIZE; i++)
		data[i] = i;
	packageTest(PSTR("Sending test data..."), COM_W_SEND, BUFFER_SIZE, data);
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
