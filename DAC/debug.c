#include "debug.h"

int uputchar0(char c, FILE *stream)
{
	if (c == '\n') uputchar0('\r', stream);
	while (!(UCSR0A & _BV(UDRE0)));
	UDR0 = c;
	return c;
}

int ugetchar0(FILE *stream)
{
	while(!(UCSR0A & _BV(RXC0)));
	return UDR0;
}
char get_ch(void)
{
	while(!(UCSR0A & _BV(RXC0)));
	return UDR0;
}
void initi_ADC(void)
	{
		//64 division factor
		//ADCSRA |=_BV(ADPS2) |_BV(ADPS1);
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
void init_debug_uart0(void)
{
	/* Configure UART0 baud rate, one start bit, 8-bit, no parity and one stop bit */
	UBRR0H = (F_CPU/(DEBUG_BAUD*16L)-1) >> 8;
	UBRR0L = (F_CPU/(DEBUG_BAUD*16L)-1);
	UCSR0B = _BV(RXEN0) | _BV(TXEN0);
	UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);

	/* Setup new streams for input and output */
	static FILE uout = FDEV_SETUP_STREAM(uputchar0, NULL, _FDEV_SETUP_WRITE);
	static FILE uin = FDEV_SETUP_STREAM(NULL, ugetchar0, _FDEV_SETUP_READ);

	/* Redirect all standard streams to UART0 */
	stdout = &uout;
	stderr = &uout;
	stdin = &uin;
}
void init_adc(void)
{
	ADCSRA |= _BV(ADEN) | _BV(ADATE); //| _BV(ADPS2) | _BV(ADPS1); /* enables ADC set bits to set pre-scaler to /64 */
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

void init_tone(void)
{
    DDRD |= _BV(PD5);
	PORTD &= ~_BV(PD5);
	/* MODE 9 phase and frequency correct */
	TCCR1A = 0b01000001; TCCR1B = 0b00010010; // divide by 8 prescaler
}