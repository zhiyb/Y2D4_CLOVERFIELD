/*
 * Author: Yubo Zhi (yz39g13@soton.ac.uk)
 */

#include "common.h"
#include "pin.h"

#define TEXT_ZOOM	2
#define TEXT_X(len)	((tft.width() - FONT_WIDTH * TEXT_ZOOM * len) / 2)
#define TEXT_Y		(tft.height() / 3 - FONT_HEIGHT * TEXT_ZOOM / 2)
#define TEXT_COLOUR	0x667F

#define PIN_ZOOM	5
#define PIN_X		(FRAME_X + FRAME_SIZE + FRAME_SPACE + PIN_ZOOM / 2)
#define PIN_Y		(tft.height() * 2 / 3 - FONT_HEIGHT * PIN_ZOOM / 2)
#define PIN_W		(FONT_WIDTH * PIN_ZOOM)
#define PIN_H		(FONT_HEIGHT * PIN_ZOOM)
#define PIN_SPACE	FRAME_W
#define PIN_COLOUR	Grey

#define FRAME_SIZE	2
#define FRAME_SPACE	4
#define FRAME_X		((tft.width() - PIN_SPACE * PIN_LENGTH) / 2)
#define FRAME_Y		(PIN_Y - FRAME_SPACE - FRAME_SIZE - PIN_ZOOM / 2)
#define FRAME_W		(PIN_W + FRAME_SPACE * 2 + FRAME_SIZE * 2)
#define FRAME_H		(PIN_H + FRAME_SPACE * 2 + FRAME_SIZE * 2)
#define FRAME_CLR	White

uint16_t EEMEM pin_t::NVpin;

bool pin_t::init(void)
{
	if (!eeprom_first()) {
		eeprom_read_block(&pin, &NVpin, sizeof(NVpin));
		return true;
	}
	return false;
}

void pin_t::set(uint16_t v)
{
	pin = v;
	eeprom_update_block(&pin, &NVpin, sizeof(NVpin));
}

void pin_t::display(const char *str)
{
	tft.setBackground(Black);
	tft.setForeground(TEXT_COLOUR);
	tft.clean();
	tft.setZoom(TEXT_ZOOM);

	tft.setXY(TEXT_X(strlen_P(str)), TEXT_Y);
	tft.putString(str, true);

	uint16_t x = FRAME_X, y = FRAME_Y;
	for (uint8_t i = 0; i < PIN_LENGTH; i++) {
		tft.frame(x, y, FRAME_W, FRAME_H, FRAME_SIZE, FRAME_CLR);
		x += FRAME_W;
	}

	tft.setZoom(PIN_ZOOM);
	tft.setForeground(PIN_COLOUR);
	pos = 0;
}

void pin_t::put(uint8_t c)
{
	if (pos == PIN_LENGTH)
		return;
	ip[pos] = c;
	tft.setXY(PIN_X + PIN_SPACE * pos, PIN_Y);
#ifdef PIN_DISPLAY
	tft << (c + '0');
#else
	tft << '*';
#endif
	pos++;
}

void pin_t::remove(void)
{
	if (!pos)
		return;
	pos--;
	tft.rectangle(PIN_X + PIN_SPACE * pos, PIN_Y, PIN_W, PIN_H, Black);
}

uint16_t pin_t::input(void) const
{
	uint16_t d = 0, multi = 1;
	for (uint8_t i = PIN_LENGTH; i != 0; i--) {
		d += multi * ip[i - 1];
		multi *= 10;
	}
	return d;
}

void pin_t::restart(void)
{
	while (pos)
		remove();
}

package_t *pin_t::pool(package_t *pkg)
{
	uint8_t c = keypad.pool();
	if (c == KEYPAD_NA)
		return pkg;
	if (c > 9)
		remove();
	else
		put(c);
	return pkg;
}
