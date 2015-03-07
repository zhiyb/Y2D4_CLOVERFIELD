#include <colours.h>
#include "sketch.h"
#include "common.h"

const uint16_t sketch_t::colours[8] = {Red, Green, Blue, Yellow, Cyan, Magenta, White, Black};

void sketch_t::init(void)
{
	clr = White;

	tft.setForeground(0x667F);
	tft.setBackground(Black);
	tft.setZoom(1);
	tft.clean();
}

bool sketch_t::pool(void)
{
	if (touch.pressed()) {
		rTouch::coord_t res = touch.position();
		if (res.x <= -20) {
			if (-res.x >= (int16_t)tft.width())
				return false;
			int16_t spl = tft.height() / 7;
			clr = colours[res.y / spl];
		} else if (res.x > (int16_t)(tft.width() + 20))
			tft.clean();
		else
			tft.point(res.x, res.y, clr);
	}
	return true;
}
