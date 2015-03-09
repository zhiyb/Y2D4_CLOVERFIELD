/*
 *  Author: Yubo Zhi (yz39g13@soton.ac.uk)
 */

#ifndef SKETCH_H
#define SKETCH_H

#include <tft.h>
#include <rtouch.h>
#include <communication.h>
#include "package.h"

class sketch_t
{
public:
	void init(void);
	package_t *pool(package_t *pkg);
	void setShared(bool s) {sh = s;}
	bool shared(void) const {return sh;}

private:
	void clean(void);
	bool packageHandle(package_t *pkg);
	void sendPackage(void);
	void sendCleanPackage(void);
	void writeBuffer(uint16_t x, uint16_t y);

	pkgSketch_t buffer;
	uint8_t bufferSize;

	rTouch::coord_t prev;
	uint16_t clr;
	uint8_t size;
	bool pressed, sh;
};

#endif
