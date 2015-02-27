#include <util/delay.h>
#include "dac.h"

int main(void)
{
	init_dac();

	uint8_t dac = 0;
inc:
	set_dac(dac);
	_delay_ms(5);
	if (++dac != 255)
		goto inc;
dec:
	set_dac(dac);
	_delay_ms(5);
	if (--dac != 0)
		goto dec;
	goto inc;

	return 1;
}
