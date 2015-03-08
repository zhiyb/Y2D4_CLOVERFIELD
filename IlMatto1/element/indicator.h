#ifndef INDICATOR_H
#define INDICATOR_H

#define INDICATOR_SIZE	LIST_TOP_RESERVED

#include <inttypes.h>

namespace indicator
{
	void init(void);

	// Existence checking
	void checkIlMatto2(bool detailed);
	void checkRemote(bool detailed);

	// String in PROGMEM
	void refresh(const uint16_t clr, const char *str);

	// Colour picker display
	void colourPicker(const uint16_t clr, uint8_t size, const bool refresh = true);
}

#endif
