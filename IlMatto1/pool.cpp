#include <colours.h>
#include <communication.h>
#include "pool.h"
#include "tick.h"
#include "structures.h"
#include "uart0.h"

using namespace colours::b16;

static volatile bool pingChk = false;

static void packageHandle(void)
{
	package_t *pkg = uart0_rxPackage();
	if (!pkg)
		return;
	if (pkg->command == COM_PING) {
		pkg->valid = 0;
		uart0_received();
	}
}

static void ping(void)
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

static void pingCheck(void)
{
	if (!pingChk)
		return;
	uint16_t clr;
	if (uart0_ack())
		clr = Blue;
	else {
		uart0_reset();
		clr = Red;
	}
	tft.rectangle(0, 0, 4, 4, clr);
	pingChk = false;
}

void pool(void)
{
	packageHandle();
	if (tick() == 100)
		ping();
	else if (tick() == 110)
		pingCheck();
}
