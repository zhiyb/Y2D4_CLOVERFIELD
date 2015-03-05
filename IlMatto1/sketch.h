#ifndef SKETCH_H
#define SKETCH_H

#include <tft.h>
#include <rtouch.h>

class sketch_t
{
public:
	sketch_t(tft_t *tft, rTouch *touch) : tft(tft), touch(touch) {}
	void init(void);
	bool pool(void);

private:
	tft_t *tft;
	rTouch *touch;
	uint16_t clr;
	static const uint16_t colours[8];
};

#endif
