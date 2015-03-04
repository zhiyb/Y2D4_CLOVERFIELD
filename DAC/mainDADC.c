#include <util/delay.h>
#include "dac.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "debug.h"
#include "adc.h"
#include <ctype.h>

volatile uint8_t val=0;
int main(void)
{
	init_dac();
	initi_ADC(); 
	adc_start();
	sei();
	while(1){
	}
	return 1;
}

ISR(ADC_vect)
	{
		//val = ADCH;
		set_dac(ADCH);
	}
