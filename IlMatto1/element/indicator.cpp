#include "indicator.h"
#include "common.h"

#define STATUS_X	(0)
#define STATUS_TEXT_W	(tft.width() - INDICATOR_SIZE - STATUS_X)
#define COLOURPICKER_X	(INDICATOR_SIZE)

void indicator::init(void)
{
}

void indicator::refresh(const uint16_t clr, const char *str)
{
	tft.rectangle(STATUS_X, 0, INDICATOR_SIZE, INDICATOR_SIZE, clr);
	if (!str)
		return;
	tft.rectangle(STATUS_X + INDICATOR_SIZE, 0, STATUS_TEXT_W, INDICATOR_SIZE, Black);
	tft.setZoom(1);
	tft.setForeground(clr);
	tft.setBackground(Black);
	tft.setXY(INDICATOR_SIZE + INDICATOR_SIZE / 2, 0);
	tft.putString(str, true);
}

void indicator::colourPicker(const uint16_t clr, uint8_t size, const bool refresh)
{
	if (size > INDICATOR_SIZE)
		size = INDICATOR_SIZE;
	if (refresh)
		tft.rectangle(COLOURPICKER_X, 0, INDICATOR_SIZE, INDICATOR_SIZE, Black);
	tft.rectangle(COLOURPICKER_X + (INDICATOR_SIZE - size) / 2, (INDICATOR_SIZE - size) / 2, size, size, clr);
}

void indicator::checkIlMatto2(bool detailed)
{
	if (tick() < TICK_PING_CHECK)
		return;
	if (!status.exist.IlMatto2Updated)
		return;
	if (status.exist.IlMatto2)
		indicator::refresh(Blue, detailed ? PSTR("") : 0);
	else
		indicator::refresh(Red, detailed ? PSTR("Il Matto 2 not exist!") : 0);
}

void indicator::checkRemote(bool detailed)
{
	if (tick() < TICK_PING_REMOTE_CHECK)
		return;
	if (!status.exist.remoteUpdated)
		return;
	if (status.exist.IlMatto2) {
		if (status.exist.remote)
			indicator::refresh(Green, detailed ? PSTR("") : 0);
		else
			indicator::refresh(Blue, detailed ? PSTR("Remote not exist!") : 0);
	} else
		indicator::refresh(Red, detailed ? PSTR("Il Matto 2 not exist!") : 0);
}
