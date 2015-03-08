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
	if (buf->s.type != PKG_TYPE_SKETCH)
		return false;
	uint8_t s = buf->s.size;
	uint16_t c = buf->s.clr;
	for (uint8_t i = 0; i < pkg->length - PKG_SKETCH_PREPEND; i++) {
		uint16_t x = buf->s.pos[i / 2][0];
		uint16_t y = buf->s.pos[i / 2][1];
		tft.rectangle(x - s / 2, y - s / 2, s, s, c);
	}
	return true;
}

void sketch_t::sendPackage(void)
{
	if (bufferSize == PKG_SKETCH_PREPEND)
		return;
	buffer.s.clr = clr;
	buffer.s.size = size;

	package_t *pkg;
	while (!(pkg = uart0_txPackage()));

	pkg->command = COM_W_SEND;
	pkg->length = bufferSize;
	for (uint8_t i = 0; i < bufferSize; i++)
		pkg->data[i] = buffer.d[i];
	pkg->valid++;
	uart0_send();

	bufferSize = PKG_SKETCH_PREPEND;
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
			clean();
			bufferSize = PKG_SKETCH_PREPEND;
		} else {
			tft.rectangle(pos.x - size / 2, pos.y - size / 2, size, size, clr);

			if (shared()) {
				uint8_t i = (bufferSize - PKG_SKETCH_PREPEND) / 4;
				buffer.s.pos[i][0] = pos.x;
				buffer.s.pos[i][1] = pos.y;
				bufferSize += 4;
#if ((BUFFER_SIZE - PKG_SKETCH_PREPEND) % 4)
				if (bufferSize + 4 > BUFFER_SIZE)
#else
				if (bufferSize + PKG_SKETCH_PREPEND == BUFFER_SIZE)
#endif
					sendPackage();
			}
		}
	} else if (pressed) {
		pressed = false;
		uint8_t i = keypad.translate(keypad.keyAt(prev));
		if (i > 0 && i < 0x0A) {
			if (shared())
				sendPackage();
			size = i;
		}
		indicator::colourPicker(clr, size);
	}
	return pkg;
}
