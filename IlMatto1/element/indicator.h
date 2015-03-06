#ifndef INDICATOR_H
#define INDICATOR_H

#define INDICATOR_SIZE	LIST_TOP_RESERVED

#include <inttypes.h>

namespace indicator
{
	void init(void);
	// Pooling for ping
	void pool(void);
	// String in PROGMEM
	void refresh(uint16_t clr, const char *str);
}

#endif
