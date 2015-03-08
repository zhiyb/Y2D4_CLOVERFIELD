/*
 *  Author: Yubo Zhi (yz39g13@soton.ac.uk)
 */

#include <colours.h>
#include "sketch.h"
#include "common.h"

void sketch_t::init(void)
{
	clr = White;
	size = 1;
	pressed = false;
	bufferSize = PKG_SKETCH_PREPEND;
	buffer.s.type = PKG_TYPE_SKETCH;

	clean();
}

void sketch_t::clean(void)
{
	tft.setBackground(Black);
	tft.clean();
	indicator::colourPicker(clr, size);
}

bool sketch_t::packageHandle(package_t *pkg)
{
	if (!pkg || (pkg->command != COM_W_RECV && pkg->command != COM_W_SEND))
		return false;
	buffer_t *buf = (buffer_t *)pkg->data;
	if (buf->s.type != PKG_TYPE_SKETCH && buf->s.type != PKG_TYPE_SKETCH_CLEAN)
		return false;
	if (buf->s.type == PKG_TYPE_SKETCH_CLEAN) {
		clean();
		return true;
	}
	uint8_t s = buf->s.size;
	uint16_t c = buf->s.clr;
	for (uint8_t i = 0; i * 4 < pkg->length - PKG_SKETCH_PREPEND; i++) {
		uint16_t x = buf->s.pos[i][0];
		uint16_t y = buf->s.pos[i][1];
		tft.rectangle(x - s / 2, y - s / 2, s, s, c);
	}
	return true;
}

void sketch_t::sendCleanPackage(void)
{
	package_t *pkg;
	uint16_t t = tick() ? tick() - 1 : TICK_CYCLE;
	while (!(pkg = uart0_txPackage()))
		if (tick() == t)
			uart0_reset();

	pkg->command = COM_W_SEND;
	pkg->length = 1;
	pkg->data[0] = PKG_TYPE_SKETCH_CLEAN;
	pkg->valid++;
	uart0_send();
}

void sketch_t::sendPackage(void)
{
	if (bufferSize == PKG_SKETCH_PREPEND)
		return;
	buffer.s.clr = clr;
	buffer.s.size = size;

	package_t *pkg;
	//uint16_t t = tick() ? tick() - 1 : TICK_CYCLE;
	while (!(pkg = uart0_txPackage()));
	//	if (tick() == t)
	//		uart0_reset();

	pkg->command = COM_W_SEND;
	pkg->length = bufferSize;
	for (uint8_t i = 0; i < bufferSize; i++)
		pkg->data[i] = buffer.d[i];
	pkg->valid++;
	uart0_send();

	bufferSize = PKG_SKETCH_PREPEND;
}

void sketch_t::writeBuffer(uint16_t x, uint16_t y)
{
	uint8_t idx = (bufferSize - PKG_SKETCH_PREPEND) / 4;

	for (uint8_t i = 0; i < idx; i++)
		if (buffer.s.pos[i][0] == x && buffer.s.pos[i][1] == y)
			return;

	buffer.s.pos[idx][0] = x;
	buffer.s.pos[idx][1] = y;
	bufferSize += 4;
#if ((BUFFER_SIZE - PKG_SKETCH_PREPEND) % 4)
	if (bufferSize + 4 > BUFFER_SIZE)
#else
	if (bufferSize == BUFFER_SIZE)
#endif
		sendPackage();
}

package_t *sketch_t::pool(package_t *pkg)
{
	if (shared() && packageHandle(pkg)) {
		uart0_done(pkg);
		pkg = 0;
	}
	rTouch::Status s = touch.status();
	if (s != rTouch::Idle) {
		rTouch::coord_t pos = touch.position();
		if (pos.x < 0) {
			if (s == rTouch::Pressed)
				pressed = true;
			else {
				pressed = false;
				if (keypad.colourPicker(pos, clr)) {
					if (shared())
						sendPackage();
					indicator::colourPicker(clr, size, false);
				}
			}
			prev.x = pos.x;
			prev.y = pos.y;
		} else if (pos.x > (int16_t)(tft.width() + 10)) {
			sendCleanPackage();
			clean();
			bufferSize = PKG_SKETCH_PREPEND;
		} else {
			tft.rectangle(pos.x - size / 2, pos.y - size / 2, size, size, clr);
			if (shared())
				writeBuffer(pos.x, pos.y);
		}
	} else {
		if (shared())
			sendPackage();
		if (pressed) {
			pressed = false;
			uint8_t i = keypad.translate(keypad.keyAt(prev));
			if (i > 0 && i < 0x0A)
				size = i;
			indicator::colourPicker(clr, size);
		}
	}
	return pkg;
}
