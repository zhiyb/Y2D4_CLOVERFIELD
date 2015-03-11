/*
 *  Author: Yubo Zhi (yz39g13@soton.ac.uk)
 */

#include <avr/io.h>
#include <avr/pgmspace.h>
#include "menu.h"
#include "pool.h"
#include "common.h"

#ifdef DEBUG
namespace menu
{
	namespace diagnosis
	{
		static void packageTest(const char *name, uint8_t command, uint8_t length = 0, const uint8_t *data = 0);
	}
}
#endif

using namespace menu;

bool menu::toggle::func(bool enter)
{
	PINB |= _BV(7);
	return false;
}

bool menu::lock::func(bool enter)
{
	tft.vsNormal();
	pool::pinLock();
	return false;
}

bool menu::audio::func(bool enter)
{
	tft.vsNormal();
	if (pool::sendRequest(PKG_REQUEST_AUDIO))
		pool::audio();
	return false;
}

#ifdef DEBUG
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
	if (!name) {
		pkg = status.pool(::pool::pool());

		status.checkRemote();
		indicator::checkRemote(false);
	} else
		pkg = uart0_rxPackage();
	if (pkg) {
		if (name) {
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
		}
		uart0_done(pkg);
	}

	if (name && uart0_ack()) {
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

bool menu::diagnosis::pool_quiet::func(bool enter)
{
	packageTest(0, 0);
	return false;
}

bool menu::diagnosis::pool::func(bool enter)
{
	packageTest(PSTR(""), 0);
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

bool menu::diagnosis::w_audio_tx::func(bool enter)
{
	packageTest(PSTR("Enabling audio TX..."), COM_W_AUDIO_TX);
	return false;
}

bool menu::diagnosis::w_audio_stop::func(bool enter)
{
	packageTest(PSTR("Disabling audio transceiver..."), COM_W_AUDIO_TX_END);
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

bool menu::diagnosis::w_data::func(bool enter)
{
	uint8_t data[BUFFER_SIZE];
	for (uint8_t i = 0; i < BUFFER_SIZE; i++)
		data[i] = i;

	tft.vsNormal();
	tft.setForeground(0x667F);
	tft.setBackground(Black);
	tft.setZoom(1);
	tft.clean();

	puts_P(PSTR("*** Il Matto1 testing ***"));
	struct package_t *pkg;

	for (uint8_t i = 0; i < 10; i++) {
		puts_P(PSTR("Alloc tx package..."));
		while (!(pkg = uart0_txPackage()));
		puts_P(PSTR("Send package..."));
		pkg->command = COM_W_SEND;
		pkg->length = BUFFER_SIZE - i;
		for (uint8_t i = 0; i < BUFFER_SIZE; i++)
			pkg->data[i] = *(data + i);
		pkg->valid++;
		uart0_send();
	}

	puts_P(PSTR("Done."));
	touch.waitForPress();

	return false;
}

bool menu::diagnosis::keypad::func(bool enter)
{
	tft.vsNormal();
	::pool::testKeypad();

	return false;
}

bool menu::diagnosis::text::func(bool enter)
{
	tft.vsNormal();
	char str[PKG_TEXT_LENGTH];
	::pool::textInput(PSTR("Text input\n"), str);

	return false;
}
#endif

bool menu::sketch::single::func(bool enter)
{
	tft.vsNormal();
	pool::sketch(false);
	return false;
}

bool menu::sketch::shared::func(bool enter)
{
	tft.vsNormal();
	if (pool::sendRequest(PKG_REQUEST_SKETCH))
		pool::sketch(true);
	return false;
}

bool menu::settings::reset_pin::func(bool enter)
{
	tft.vsNormal();
	pool::pinLock();
	pool::pinSet();
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

bool menu::settings::frequency::func(bool enter)
{
	return false;
}

bool menu::game::tictactoe::func(bool enter)
{
	return false;
}

bool menu::text::func(bool enter)
{
	tft.vsNormal();
	char str[PKG_TEXT_LENGTH];
	if (pool::textInput(PSTR("Write message:\n"), str) && *str)
		pool::sendMessage(str);
	return false;
}
