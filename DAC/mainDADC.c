#include <util/delay.h>
#include "dac.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "debug.h"
#include <ctype.h>

volatile uint8_t val=0;
int main(void)
{
	init_dac();
	//init_debug_uart0();
	initi_ADC(); 
	adc_start();
	sei();
	//char ch;
	while(1){
		//ch = get_ch();
		//set_dac(val);
		//set_dac(ch);
		//printf("%d",ADCH);
	}
	return 1;
}

ISR(ADC_vect)
	{
		//val = ADCH;
		set_dac(ADCH);
	}
