#ifndef SKETCH_H
#define SKETCH_H

#include <tft.h>
#include <rtouch.h>

class sketch_t
{
public:
	void init(void);
	bool pool(void);

private:
	uint16_t clr;
	static const uint16_t colours[8];
};

#endif
