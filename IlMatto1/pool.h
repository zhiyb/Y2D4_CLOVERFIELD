#ifndef POOL_H
#define POOL_H

#include <communication.h>

namespace pool
{
	void init(void);
	// Detailed indicator (ping check)
	package_t *pool(void);
}

#endif
