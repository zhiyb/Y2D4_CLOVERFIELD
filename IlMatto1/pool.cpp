/*
 *  Author: Yubo Zhi (yz39g13@soton.ac.uk)
 */

#include "pool.h"
#include "common.h"

#define AUDIO_TEXT_X	((tft.width() - FONT_WIDTH * 2 * 13) / 2)
#define AUDIO_TEXT_Y	(tft.height() / 3 - FONT_HEIGHT * 2 / 2)
#define AUDIO_SIZE	128
#define AUDIO_X		(tft.width() / 2 - AUDIO_SIZE / 2)
#define AUDIO_Y		(tft.height() * 2 / 3 - AUDIO_SIZE / 2)
#define AUDIO_CLR	DarkRed
#define AUDIO_CLR_ACT	Red

#define TEXT_ZOOM	2

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
			if (notification.requestAck(PKG_REQUEST_SKETCH) == PKG_REQUEST_CLOSED) {
				indicator::refresh(Blue, PSTR("Opponent closed!"));
				shared = false;
			}
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
	if (shared)
		notification.sendRequestAck(PKG_REQUEST_SKETCH, PKG_REQUEST_CLOSED);
}

#ifdef DEBUG
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
#endif

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
		if (notification.requestAck(req) == PKG_REQUEST_CLOSED)
			return;
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
	case PKG_REQUEST_AUDIO:
		pool::audio();
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
	bool pressed = false;
	for (;;) {
		if (t == tick()) {
			notification.sendRequest(req);
			if (!t--)
				t = TICK_CYCLE;
		}
		uart0_done(notification.pool(status.pool(::pool::pool())));
		if ((ack = notification.requestAck(req)) != PKG_REQUEST_INVALID)
			break;
		if (touch.pressed())
			pressed = true;
		else if (pressed)
			goto close;
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
		if (touch.pressed())
			pressed = true;
		else if (pressed)
			goto close;
	}

reject:
	tft.setForeground(Red);
	tft.putString(PSTR("REJECTED!\n"), true);
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

close:
	notification.sendRequestAck(req, PKG_REQUEST_CLOSED);
	return false;
}

void pool::audio(void)
{
	tft.setBackground(Black);
	tft.setForeground(0x667F);
	tft.clean();
	tft.setZoom(2);
	tft.setXY(AUDIO_TEXT_X, AUDIO_TEXT_Y);
	tft.putString(PSTR("Press to talk"), true);

	tft.rectangle(AUDIO_X, AUDIO_Y, AUDIO_SIZE, AUDIO_SIZE, AUDIO_CLR);

	bool pressed = false;
	package_t *pkg;
	for (;;) {
		uart0_done(notification.pool(status.pool(::pool::pool())));
		if (notification.requestAck(PKG_REQUEST_AUDIO) == PKG_REQUEST_CLOSED)
			goto ret;
		if (touch.pressed()) {
			rTouch::coord_t pos = touch.position();
			if (keypad.outsideLeft(pos.x + 10)) {
				notification.sendRequestAck(PKG_REQUEST_AUDIO, PKG_REQUEST_CLOSED);
				goto ret;
			}
			if (!pressed) {
				tft.rectangle(AUDIO_X, AUDIO_Y, AUDIO_SIZE, AUDIO_SIZE, AUDIO_CLR_ACT);
				pressed = true;
			}
			while (!(pkg = uart0_txPackage()));
			pkg->command = COM_W_AUDIO_TX;
			pkg->valid++;
			uart0_send();
		} else if (pressed) {
			tft.rectangle(AUDIO_X, AUDIO_Y, AUDIO_SIZE, AUDIO_SIZE, AUDIO_CLR);
			pressed  = false;
			while (!(pkg = uart0_txPackage()));
			pkg->command = COM_W_AUDIO_TX_END;
			pkg->valid++;
			uart0_send();
		}
	}
ret:
	if (pressed) {
		while (!(pkg = uart0_txPackage()));
		pkg->command = COM_W_AUDIO_TX_END;
		pkg->valid++;
		uart0_send();
	}
}

bool pool::textInput(const char *str, char *buf)
{
	tft.setBackground(Black);
	tft.setForeground(0x667F);
	tft.clean();
	tft.setZoom(TEXT_ZOOM);
	tft.putString(str, true);
	keypad.initText();
	tft.setForeground(White);
	tft.rectangle(tft.x(), tft.y(), FONT_WIDTH * TEXT_ZOOM, FONT_HEIGHT * TEXT_ZOOM, tft.foreground());

	uint8_t len = 0;
	for (;;) {
		uart0_done(notification.pool(status.pool(::pool::pool())));
		if (touch.pressed()) {
			rTouch::coord_t pos = touch.position();
			if (pos.x >= 0) {
				*buf = 0;
				return true;
			}
		}
		while (notification.show());
		char c = keypad.text();
		if (c == -1)
			continue;
		if (c == KEYPAD_DEL) {
			if (!len)
				continue;
			buf--;
			len--;
			if (tft.x())
				tft.setX(tft.x() - FONT_WIDTH * TEXT_ZOOM);
			else {
				tft.setX(tft.width() - FONT_WIDTH * TEXT_ZOOM);
				tft.setY(tft.y() - FONT_HEIGHT * TEXT_ZOOM);
			}
			tft.rectangle(tft.x(), tft.y(), FONT_WIDTH * TEXT_ZOOM, FONT_HEIGHT * TEXT_ZOOM, tft.background());
		} else if (len != PKG_TEXT_LENGTH - 1) {
			tft << c;
			*buf++ = c;
			len++;
			tft.rectangle(tft.x(), tft.y(), FONT_WIDTH * TEXT_ZOOM, FONT_HEIGHT * TEXT_ZOOM, tft.foreground());
		}
	}
	return false;
}

void pool::message(const char *str)
{
	notification.displayMessage(str);
	status.request.refresh = true;

	for (;;) {
		uart0_done(notification.pool(status.pool(::pool::pool())));
		status.checkIlMatto2();
		indicator::checkIlMatto2(true);
		if (notification.messagePool())
			break;
		if (touch.pressed()) {
			rTouch::coord_t pos = touch.position();
			if (keypad.outsideLeft(pos.x + 10))
				break;
		}
	}
}

void pool::sendMessage(const char *str)
{
	tft.putString(PSTR("\nSending message..."), true);

	bool pressed = false;
	uint8_t idx = notification.messageIndex();
	uint16_t t = tick();
	for (;;) {
		if (t == tick()) {
			notification.sendMessage(idx, str);
			if (!t--)
				t = TICK_CYCLE;
		}
		uart0_done(notification.pool(status.pool(::pool::pool())));
		if (notification.messageAck(idx))
			break;
		if (touch.pressed())
			pressed = true;
		else if (pressed)
			break;
	}
}

#ifdef DEBUG
void pool::tictactoe(void)
{
}
#endif
