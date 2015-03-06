#include <colours.h>
#include "sketch.h"
#include "common.h"

//#define AUTO_COLOUR

const uint16_t sketch_t::colours[8] = {Red, Green, Blue, Yellow, Cyan, Magenta, White, Black};

void sketch_t::init(void)
{
	clr = White;

	tft.vsNormal();
	tft.setForeground(0x667F);
	tft.setBackground(Black);
	tft.setZoom(1);
	tft.clean();
}

bool sketch_t::pool(void)
{
#ifdef AUTO_COLOUR
	bool pressed = false;
	uint8_t cnt = 0;
#endif
	if (touch.pressed()) {
		rTouch::coord_t res = touch.position();
		if (res.x <= -20) {
			if (-res.x >= (int16_t)tft.width())
				return false;
#if 1
			int16_t spl = tft.height() / 7;
			clr = colours[res.y / spl];
#else
			uint8_t mv = res.y * 16 / tft->height();
			clr = (0x001F << mv) | (((uint32_t)0x001F << mv) >> 16);
#endif
		} else if (res.x > (int16_t)(tft.width() + 20))
			tft.clean();
		else {
#ifdef AUTO_COLOUR
			pressed = true;
#endif
			tft.point(res.x, res.y, clr);
		}
#ifdef AUTO_COLOUR
	} else if (pressed) {
		clr = c[cnt++];
		if (cnt == 7)
			cnt = 0;
		pressed = false;
#endif
	}
	return true;
}
