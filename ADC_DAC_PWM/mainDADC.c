#include <util/delay.h>
#include "dac.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "debug.h"
#include "adc.h"
#include "pwm.h"
#include <ctype.h>

volatile uint8_t val = 0;
int main(void)
{
	init_dac();
	initi_ADC();
	adc_start();
	pwm_init();
	pwm_enable(1);
	sei();
	while (1) {
	}
	return 1;
}

ISR(ADC_vect)
{
	//val = ADCH;
	set_dac(ADCH);
	pwm_set(ADCH);
}
