#include "pool.h"
#include "common.h"

using namespace colours::b16;

package_t *pool::pool(void)
{
	if (tick() == TICK_CLEAR)
		while (uart0_ack());
	return uart0_rxPackage();
}
