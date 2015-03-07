#include "indicator.h"
#include "common.h"

namespace indicator
{
	// Ping related
	static volatile bool pingChk;
	static void ping(void);
	static void pingCheck(bool detailed);
}

void indicator::init(void)
{
	pingChk = false;
}

void indicator::refresh(uint16_t clr, const char *str)
{
	tft.rectangle(0, 0, INDICATOR_SIZE, INDICATOR_SIZE, clr);
	if (!str)
		return;
	tft.rectangle(INDICATOR_SIZE, 0, tft.width() - INDICATOR_SIZE, INDICATOR_SIZE, Black);
	tft.setZoom(1);
	tft.setForeground(clr);
	tft.setBackground(Black);
	tft.setXY(INDICATOR_SIZE + INDICATOR_SIZE / 2, 0);
	tft.putString(str, true);
}

static void indicator::ping(void)
{
	if (pingChk)
		return;
	PINB |= _BV(7);
	package_t *pkg = uart0_txPackage();
	if (!pkg) {
		pingChk = false;
		refresh(Magenta, PSTR("Pending tx package..."));
		return;
	}
	pkg->command = COM_PING;
	pkg->valid++;
	uart0_send();
	pingChk = true;
}

static void indicator::pingCheck(bool detailed)
{
	if (!pingChk)
		return;
	if (uart0_ack())
		refresh(Blue, detailed ? PSTR("") : 0);
	else {
		uart0_reset();
		refresh(Red, detailed ? PSTR("Il Matto 2 not exist!") : 0);
	}
	pingChk = false;
}

void indicator::pool(bool detailed)
{
	if (tick() <= 100)
		ping();
	else if (tick() >= 101)
		pingCheck(detailed);
}
