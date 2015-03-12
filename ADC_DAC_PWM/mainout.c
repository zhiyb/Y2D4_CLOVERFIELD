#include <util/delay.h>
#include "dac.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "debug.h"
#include <ctype.h>

int main(void)
{
	init_dac();
	init_debug_uart0();
	init_adc();
	adc_start();
	sei();
	char ch;
	while (1) {
		ch = get_ch();
		set_dac(ch);
	}
	return 1;
}
