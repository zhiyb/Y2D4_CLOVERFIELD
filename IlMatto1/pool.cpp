#include <colours.h>
#include <communication.h>
#include "pool.h"
#include "tick.h"
#include "common.h"
#include "uart0.h"
#include "indicator.h"

namespace pool
{
	static void packageHandle(void);
}

using namespace colours::b16;

void pool::init(void)
{
	indicator::init();
}

static void pool::packageHandle(void)
{
	package_t *pkg = uart0_rxPackage();
	if (!pkg)
		return;
	if (pkg->command == COM_PING) {
		pkg->valid = 0;
		uart0_received();
	}
}

void pool::pool(bool detailed)
{
	packageHandle();
	indicator::pool(detailed);
}
