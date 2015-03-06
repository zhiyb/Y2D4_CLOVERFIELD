#include "indicator.h"
#include "common.h"

namespace indicator
{
	static void refresh(uint16_t clr);

	// Ping related
	static volatile bool pingChk;
	static void ping(void);
	static void pingCheck(void);
}

void indicator::init(void)
{
	pingChk = false;
}

static void indicator::refresh(uint16_t clr)
{
	tft.rectangle(0, 0, INDICATOR_SIZE, INDICATOR_SIZE, clr);
}

static void indicator::ping(void)
{
	if (pingChk)
		return;
	package_t *pkg = uart0_txPackage();
	if (!pkg) {
		pingChk = false;
		return;
	}
	pkg->command = COM_PING;
	pkg->valid++;
	uart0_send();
	pingChk = true;
}

static void indicator::pingCheck(void)
{
	if (!pingChk)
		return;
	if (uart0_ack())
		refresh(Blue);
	else {
		uart0_reset();
		refresh(Red);
	}
	pingChk = false;
}

void indicator::pool(void)
{
	if (tick() == 100)
		ping();
	else if (tick() == 110)
		pingCheck();
}
