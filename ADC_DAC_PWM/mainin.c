#include <util/delay.h>
#include "dac.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "debug.h"
#include <ctype.h>

int main(void)
{
	//init_dac();
	init_debug_uart0();
	adc_start();
	initi_ADC();
	sei();
	while (1) {
	}
	return 1;
}

ISR(ADC_vect)
{
	printf("%d", ADCH);
}
