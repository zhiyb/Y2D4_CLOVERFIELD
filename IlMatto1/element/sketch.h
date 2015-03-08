#ifndef SKETCH_H
#define SKETCH_H

#include <tft.h>
#include <rtouch.h>
#include <communication.h>

#define PKG_SKETCH_PREPEND	4

class sketch_t
{
public:
	void init(void);
	volatile package_t *pool(volatile package_t *pkg);
	void setShared(bool s) {sh = s;}
	bool shared(void) const {return sh;}

private:
	void clean(void);
	bool packageHandle(volatile package_t *pkg);
	void sendPackage(void);
	void sendCleanPackage(void);
	void writeBuffer(uint16_t x, uint16_t y);

	union buffer_t {
		uint8_t d[BUFFER_SIZE];
		struct {
			uint8_t type;
			uint8_t size;
			uint16_t clr;
			uint16_t pos[BUFFER_SIZE - PKG_SKETCH_PREPEND][2];
		} s;
	} buffer;
	uint8_t bufferSize;

	rTouch::coord_t prev;
	uint16_t clr;
	uint8_t size;
	bool pressed, sh;
};

#endif
