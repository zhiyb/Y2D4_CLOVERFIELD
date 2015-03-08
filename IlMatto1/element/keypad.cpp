/*
 *  Author: Yubo Zhi (yz39g13@soton.ac.uk)
 */

#include <eemem.h>
#include <rgbconv.h>
#include "keypad.h"
#include "common.h"

// Calibration cross size
#define CALIB_SIZE	10

// Keypad drawing position
#define DRAW_X		16
#define DRAW_Y		40
#define DRAW_W		(tft.width() - DRAW_X * 2)
#define DRAW_H		(tft.height() - DRAW_Y - 16)
#define DRAW_KEY_W	(DRAW_W / KEYPAD_SIZE_X)
#define DRAW_KEY_H	(DRAW_H / KEYPAD_SIZE_Y)
#define DRAW_NAME_X	((DRAW_KEY_W - FONT_WIDTH * 2) / 2)
#define DRAW_NAME_Y	((DRAW_KEY_H - FONT_HEIGHT * 2) / 2)

const char keypad_t::PGMkeyName[KEYPAD_SIZE] PROGMEM = {
#if 1
	'1', '2', '3', 'A',
	'4', '5', '6', 'B',
	'7', '8', '9', 'C',
	'D', '0', 'E', 'F',
#else
	'7', '8', '9', 'A',
	'4', '5', '6', 'B',
	'1', '2', '3', 'C',
	'D', '0', 'E', 'F',
#endif
};

const uint8_t keypad_t::PGMkeyCode[KEYPAD_SIZE] PROGMEM = {
	0x01, 0x02, 0x03, 0x0A,
	0x04, 0x05, 0x06, 0x0B,
	0x07, 0x08, 0x09, 0x0C,
	0x0D, 0x00, 0x0E, 0x0F,
};

struct keypad_t::cal_t EEMEM keypad_t::NVcal;

void keypad_t::init(void)
{
	prevTest = prev = KEYPAD_NA;
}

uint8_t keypad_t::translate(uint8_t idx)
{
	return pgm_read_byte(&PGMkeyCode[idx]);
}

void keypad_t::drawCross(const rTouch::coord_t pos, uint16_t c) const
{
	tft.rectangle(pos.x - CALIB_SIZE, pos.y, CALIB_SIZE * 2, 1, c);
	tft.rectangle(pos.x, pos.y - CALIB_SIZE, 1, CALIB_SIZE * 2, c);
}

void keypad_t::display(void) const
{
	tft.setBackground(0x0000);
	tft.setForeground(0x667F);
	tft.setZoom(2);
	tft.clean();
	tft.setOrient(tft_t::Portrait);
	drawKeypad();
}

void keypad_t::calibrate(bool reset)
{
	if (!reset && !eeprom_first()) {
		eeprom_read_block(&cal, &NVcal, sizeof(NVcal));
		return;
	}

	display();
	tft.setXY(0, 0);
	tft.setZoom(1);
	tft.putString(PSTR("Keypad Calibration\n"), true);
	tft.setZoom(2);

	tft.putString(PSTR("Press "), true);
	tft.setForeground(White);
	tft.putString(PSTR("TOP-LEFT"), true);

	drawCross(coordinate(0), White);
	cal.pos = touch.waitForPress();
	drawCross(coordinate(0), Black);
	drawKey(0);

	tft.setXY(0, FONT_HEIGHT);
	tft.putString(PSTR("Press "), true);
	tft.setForeground(White);
	tft.putString(PSTR("BOTTOM-RIGHT"), true);

	rTouch::coord_t pos = coordinate(KEYPAD_SIZE + KEYPAD_SIZE_X - 1);
	pos.x += DRAW_KEY_W;
	drawCross(pos, White);
	cal.size = touch.waitForPress();
	cal.size.x -= cal.pos.x;
	cal.size.y -= cal.pos.y;

	eeprom_update_block(&cal, &NVcal, sizeof(NVcal));
}

rTouch::coord_t keypad_t::coordinate(uint8_t idx) const
{
	rTouch::coord_t pos;
	pos.x = (idx % KEYPAD_SIZE_X) * DRAW_KEY_W + DRAW_X;
	pos.y = (idx / KEYPAD_SIZE_X) * DRAW_KEY_H + DRAW_Y;
	return pos;
}

void keypad_t::drawKey(uint8_t idx, uint16_t clr) const
{
	rTouch::coord_t pos = coordinate(idx);
	tft.setForeground(clr);
	tft.frame(pos.x, pos.y, DRAW_KEY_W, DRAW_KEY_H, 1, clr);
	tft.setXY(pos.x + DRAW_NAME_X, pos.y + DRAW_NAME_Y);
	tft << (char)pgm_read_byte(&PGMkeyName[idx]);
}

void keypad_t::drawKeypad(void) const
{
	for (uint8_t i = 0; i < KEYPAD_SIZE; i++)
		drawKey(i);
}

uint8_t keypad_t::keyAt(const rTouch::coord_t pos) const
{
	if (outside(pos))
		return KEYPAD_NA;
	return (pos.y - cal.pos.y) * KEYPAD_SIZE_Y / cal.size.y * KEYPAD_SIZE_X + (pos.x - cal.pos.x) * KEYPAD_SIZE_X / cal.size.x;
}

uint8_t keypad_t::pool(bool keep, bool code)
{
	uint8_t idx;
	if (touch.pressed()) {
		idx = keyAt(touch.position());
		if (keep || idx != prev)
			goto ret;
		return KEYPAD_NA;
	} else
		idx = KEYPAD_NA;
ret:
	prev = idx;
	return code ? translate(idx) : idx;
}

bool keypad_t::testPool(void)
{
	if (touch.pressed())
		if (touch.position().x >= 0) {
			prevTest = prev = KEYPAD_NA;
			return false;
		}
	tft.setZoom(2);
	uint8_t idx = pool(true, false);
	if (idx != prevTest) {
		if (prevTest != KEYPAD_NA)
			drawKey(prevTest);
		tft.setXY(DRAW_X, DRAW_Y - FONT_HEIGHT * 2);
		if (idx != KEYPAD_NA) {
			uint8_t code = translate(idx);
			tft << (code < 0x0A ? code + '0' : code - 0x0A + 'A');
			drawKey(idx, KEYPAD_ACT_CLR);
		} else
			tft << " ";
	}
	prevTest = idx;
	return true;
}

bool keypad_t::outside(const rTouch::coord_t pos) const
{
	return pos.x < cal.pos.x || pos.y < cal.pos.y || \
		pos.x >= cal.pos.x + cal.size.x || pos.y >= cal.pos.y + cal.size.y;
}

bool keypad_t::colourPicker(rTouch::coord_t pos, uint16_t &clr) const
{
	if (outside(pos))
		return false;
	uint8_t red = 0, green = 0, blue = 0;
	uint16_t y = (uint32_t)(pos.y - cal.pos.y) * 6 * 256 / (uint32_t)cal.size.y;
	uint8_t c = y % 256, reg = y / 256;
	switch (reg) {
	case 0:
		red = 0xFF;
		green = c;
		break;
	case 1:
		red = 0xFF - c;
		green = 0xFF;
		break;
	case 2:
		green = 0xFF;
		blue = c;
		break;
	case 3:
		green = 0xFF - c;
		blue = 0xFF;
		break;
	case 4:
		red = c;
		blue = 0xFF;
		break;
	case 5:
		red = 0xFF;
		blue = 0xFF - c;
		break;
	}
	uint16_t x = (uint32_t)(pos.x - cal.pos.x) * 2 * 256 / (uint32_t)cal.size.x;
	if (x / 256 == 0) {
		red = (uint16_t)red * x / 256;
		green = (uint16_t)green * x / 256;
		blue = (uint16_t)blue * x / 256;
	} else {
		x %= 256;
		red += (uint16_t)(0xFF - red) * x / 256;
		green += (uint16_t)(0xFF - green) * x / 256;
		blue += (uint16_t)(0xFF - blue) * x / 256;
	}
	clr = conv::c32to16(conv::c32(red, green, blue));
	return true;
}
