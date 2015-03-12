#include "adc.h"

void initi_ADC(void)
{
	//64 division factor
	ADCSRA |= _BV(ADPS2) | _BV(ADPS1);
	//enable auto trigger .
	ADCSRA |= _BV(ADATE);
	//auto trigger source as free running mode,its initial value is zero.
	//ADCSRB &= ~_BV(ADTS2)&~_BV(ADTS1)&~_BV(ADTS0);
	//configure the reference voltage Vref to be AVcc.
	//ADMUX |= _BV(REFS0);
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

void init_adc(void)
{
	ADCSRA |= _BV(ADEN) | _BV(ADATE);	//| _BV(ADPS2) | _BV(ADPS1); /* enables ADC set bits to set pre-scaler to /64 */
	ADMUX |= _BV(ADLAR);
}

void channel_adc(uint8_t n)
{
	ADMUX = n;
}

void adc_interrupt_enable(void)
{
	ADCSRA |= _BV(ADIE);
}

void adc_start(void)
{
	ADCSRA |= _BV(ADSC);
}

void adc_stop(void)
{
	ADCSRA &= ~_BV(ADSC);
}
