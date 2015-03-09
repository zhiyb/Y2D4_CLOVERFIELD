/*
 *  Author: Yubo Zhi (yz39g13@soton.ac.uk)
 */

#include "pool.h"
#include "common.h"

namespace pool
{
	void pin(void);
}

package_t *pool::pool(void)
{
	if (tick() == TICK_CLEAR)
		while (uart0_ack());
	return uart0_rxPackage();
}

void pool::pin(void)
{
	uart0_done(::pin.pool(status.pool(pool())));
}

void pool::pinLock(void)
{
	::pin.display(PSTR("Enter Passcode"));

	for (;;) {
		pin();

		if (::pin.done()) {
			if (::pin.matched())
				return;
			else
				::pin.restart();
		}
	}
}

void pool::pinSet(void)
{
restart:
	::pin.display(PSTR("New passcode"));

	bool verify = false;
	uint16_t v = 0;
	for (;;) {
		pin();

		if (::pin.done()) {
			if (verify) {
				if (::pin.input() != v)
					goto restart;
				::pin.set(v);
				break;
			} else {
				v = ::pin.input();
				::pin.display(PSTR("Verify passcode"));
				verify = true;
			}
		}
	}
}

void pool::sketch(bool shared)
{
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

void pool::testKeypad(void)
{
	keypad.display();

	for (;;) {
		uart0_done(status.pool(::pool::pool()));
		status.checkIlMatto2();
		indicator::checkIlMatto2(true);
		if (!keypad.testPool())
			break;
	}
}
