#include "pool.h"
#include "common.h"

namespace pool
{
	static package_t *packageHandle(void);
}

using namespace colours::b16;

void pool::init(void)
{
	indicator::init();
}

static package_t *pool::packageHandle(void)
{
	package_t *pkg = uart0_rxPackage();
	if (!pkg)
		return 0;
	return pkg;
}

package_t *pool::pool(void)
{
	if (tick() == TICK_CLEAR)
		while (uart0_ack());
	return packageHandle();
}
