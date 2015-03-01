/*
 * ADC.c
 *
 *  Created on: 1 Mar 2015
 *      Author: ak12g12
 */

/*
 * ADC.C
 *
 *  Created on: 28 Feb 2015
 *      Author: ak12g12
 */
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "debug.h"
void initi_ADC(void);
int main(void)
	{
		initi_ADC();
		init_debug_uart0();
		while(1)
		{

		}
	}
//Initialisation of the ADC
void initi_ADC(void)
	{
		//64 division factor
		ADCSRA |=_BV(ADPS2) |_BV(ADPS1);
		//enable auto trigger .
		ADCSRA |= _BV(ADATE);
		//auto trigger source as free running mode,its initial value is zero.
		//ADCSRB &= ~_BV(ADTS2)&~_BV(ADTS1)&~_BV(ADTS0);
		//configure the reference voltage Vref to be AVcc.
		ADMUX |= _BV(REFS0);
		//left adjusted result in the data registers
		ADMUX |= _BV(ADLAR);
		//enable ADC
		ADCSRA |= _BV(ADEN);
		//START CONVERSION
		ADCSRA |= _BV(ADSC);
		//enable ADC interrupt
		ADCSRA |= _BV(ADIE);
		//enable global interrupt
		sei();
	}
ISR(ADC_vect)
	{
		printf("the result of ADC conversion is %4d\n",ADCH);
		_delay_ms(1000);
	}

