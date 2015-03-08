/*
 *  Author: Yubo Zhi (yz39g13@soton.ac.uk)
 */

#include "pool.h"
#include "common.h"

package_t *pool::pool(void)
{
	if (tick() == TICK_CLEAR)
		while (uart0_ack());
	return uart0_rxPackage();
}
