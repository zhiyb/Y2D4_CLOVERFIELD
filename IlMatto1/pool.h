/*
 *  Author: Yubo Zhi (yz39g13@soton.ac.uk)
 */

#ifndef POOL_H
#define POOL_H

#include <communication.h>

namespace pool
{
	// Package fetch & basic pool
	package_t *pool(void);

	void pinLock(void);
	void pinSet(void);

	void sketch(bool shared);

	void testKeypad(void);
}

#endif
