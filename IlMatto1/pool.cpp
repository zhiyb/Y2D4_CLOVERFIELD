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
	uart0_done(notification.pool(::pin.pool(status.pool(pool()))));
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
		uart0_done(notification.pool(::sketch.pool(status.pool(pool::pool()))));
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
		uart0_done(notification.pool(status.pool(::pool::pool())));
		status.checkIlMatto2();
		indicator::checkIlMatto2(true);
		if (!keypad.testPool())
			break;
		while (notification.show());
		if (status.request.refresh) {
			keypad.display();
			status.request.refresh = false;
		}
	}
}

void pool::list(void)
{
	for (;;) {
		uart0_done(notification.pool(status.pool(::pool::pool())));
		status.checkIlMatto2();
		indicator::checkIlMatto2(true);
		::list.pool(&touch);
		if (touch.pressed()) {
			rTouch::coord_t pos = touch.position();
			if (keypad.outsideLeft(pos.x + 10))
				::list.toUpperLevel();
		}
		while (notification.show());
		if (status.request.refresh) {
			::list.refresh();
			status.request.refresh = false;
		}
	}
}

void pool::request(uint8_t req)
{
	notification.displayRequest(req);

	for (;;) {
		uart0_done(notification.pool(status.pool(::pool::pool())));
		status.checkRemote();
		indicator::checkRemote(true);
		switch (notification.requestPool()) {
		case PKG_REQUEST_ACCEPT:
			goto accept;
		case PKG_REQUEST_REJECT:
			goto reject;
		}
		if (touch.pressed()) {
			rTouch::coord_t pos = touch.position();
			if (keypad.outsideLeft(pos.x + 10))
				goto reject;
		}
	}
accept:
	notification.sendRequestAck(req, PKG_REQUEST_ACCEPT);
	switch (req) {
	case PKG_REQUEST_SKETCH:
		pool::sketch(true);
		break;
	}
	return;
reject:
	notification.sendRequestAck(req, PKG_REQUEST_REJECT);
}

bool pool::sendRequest(uint8_t req)
{
	tft.setBackground(Black);
	tft.setForeground(0x667F);
	tft.clean();
	tft.setZoom(2);
	tft.putString(PSTR("Sending request...\n"), true);

	uint8_t ack = PKG_REQUEST_INVALID;
	uint16_t t = tick();
	for (;;) {
		if (t == tick()) {
			notification.sendRequest(req);
			if (!t--)
				t = TICK_CYCLE;
		}
		uart0_done(notification.pool(status.pool(::pool::pool())));
		if ((ack = notification.requestAck(req)) != PKG_REQUEST_INVALID)
			break;
	}

	tft.putString(PSTR("Waiting response...\n"), true);
	for (;;) {
		switch (ack) {
		case PKG_REQUEST_ACCEPT:
			return true;
		case PKG_REQUEST_REJECT:
			goto reject;
		}
		uart0_done(notification.pool(status.pool(::pool::pool())));
		ack = notification.requestAck(req);
	}

reject:
	tft.setForeground(Red);
	tft.putString(PSTR("REJECTED!\n"), true);
	bool pressed = false;
	for (;;) {
		uart0_done(notification.pool(status.pool(::pool::pool())));
		while (notification.show());
		if (status.request.refresh) {
			status.request.refresh = false;
			return false;
		}
		if (touch.pressed())
			pressed = true;
		else if (pressed)
			return false;
	}

	return false;
}
