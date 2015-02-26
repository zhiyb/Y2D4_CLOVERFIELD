#include <avr/io.h>
#include <util/delay.h>
#include "debug.h"
#include <ctype.h>

int main(void){
	init_debug_uart1();
	init_adc(); 
	channel_adc(0);
	adc_interrupt_enable();
	adc_start();
	int ADC4;
	int config=0;
	DDRD |= _BV(PD3); 	//data
	DDRD |= _BV(PD2); 	//set PD0 as an output (load)
	for(;;)
	{
		_delay_ms(1000);
		PORTD |= _BV(PD2);
		
		ADC4 = ADC/4; //take ADC down to 8bit number
		while ( !(UCSR1A & (1<<UDRE1)));
		UDR1 = config;
		while ( !(UCSR1A & (1<<UDRE1)));
		UDR1 = ADC4;
		_delay_ms(1000);
		PORTD &= ~_BV(PD2);
		//ch = get_ch(); //get char from UART, wait here until done so
		//_delay_ms(1000);
		//printf("%c",ch);
	}
	
	return 0;
}
