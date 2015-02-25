#include <avr/io.h>
#include <util/delay.h>
#include "debug.h"
#include <ctype.h>

int main(void){
	init_tone(); //divide by 8 prescaler in debug.c
	init_debug_uart0();
	init_adc(); 
	channel_adc(0);
	//adc_interrupt_enable();
	adc_start();
	
	for(;;)
	{
	
		ICR1 = 0b1111111111111111; // set to 65536
		//OCR1A = 0b1000000000000000;
		OCR1A = ADC*64; // test which one when have scope and LPF constructed
		//OCR1A = 65536-(ADC*64);
		//printf("12345");
		//ch = get_ch(); //get char from UART, wait here until done so
		//_delay_ms(1000);
		//printf("%c",ch);


	}
	
	return 0;
}
