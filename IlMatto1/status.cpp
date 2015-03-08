/*
 *  Author: Yubo Zhi (yz39g13@soton.ac.uk)
 */

#include "status.h"
#include "common.h"

void status_t::init(void)
{
	pingChk = false;
	exist.IlMatto2 = false;
	exist.remote = false;
}

void status_t::ping(uint8_t cmd)
{
	if (pingChk)
		return;
	PINB |= _BV(7);
	package_t *pkg = uart0_txPackage();
	if (!pkg) {
		pingChk = false;
		return;
	}
	pkg->command = cmd;
	pkg->valid++;
	uart0_send();
	pingChk = true;
}

void status_t::pingCheck(void)
{
	if (!pingChk)
		return;
	exist.IlMatto2Updated = true;
	if (!(exist.IlMatto2 = uart0_ack()))
		uart0_reset();
	pingChk = false;
}

void status_t::pingRemoteCheck(void)
{
	if (!pingChk)
		return;
	exist.remoteUpdated = true;
	pingCheck();
}

package_t *status_t::pool(package_t *pkg)
{
	if (!pkg)
		return 0;
	switch (pkg->command) {
	case COM_W_PING_TO:
	case COM_W_PING_SU:
		exist.remote = pkg->command == COM_W_PING_SU;
	case COM_PING:
		uart0_done(pkg);
		return 0;
	case COM_W_PING: {
			package_t *tx = uart0_txPackage();
			if (tx) {
				uart0_done(pkg);
				tx->command = COM_W_PING_SU;
				tx->valid++;
				uart0_send();
			}
			return 0;
		}
	}
	return pkg;
}

void status_t::checkIlMatto2(void)
{
	if (exist.IlMatto2Updated)
		exist.IlMatto2Updated = false;
	if (tick() >= TICK_PING_CHECK)
		pingCheck();
	else if (tick() >= TICK_PING)
		ping();
}

void status_t::checkRemote(void)
{
	if (exist.remoteUpdated) {
		exist.remoteUpdated = false;
		exist.IlMatto2Updated = false;
	}
	if (tick() >= TICK_PING_REMOTE_CHECK)
		pingRemoteCheck();
	else if (tick() >= TICK_PING_REMOTE)
		ping(COM_W_PING);
}
