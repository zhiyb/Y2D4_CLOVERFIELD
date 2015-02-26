#include "debug.h"

int uputchar1(char c, FILE *stream)
{
	if (c == '\n') uputchar1('\r', stream);
	while (!(UCSR1A & _BV(UDRE1)));
	UDR1 = c;
	return c;
}

int ugetchar1(FILE *stream)
{
	while(!(UCSR1A & _BV(RXC1)));
	return UDR1;
}
char get_ch(void)
{
	while(!(UCSR1A & _BV(RXC1)));
	return UDR1;
}
void init_debug_uart1(void)
{
UBRR1 = 0;
/* Setting the XCKn port pin as output, enables master mode. */
DDRD |= _BV(4);
/* Set MSPI mode of operation and SPI data mode 0. */
UCSR1C = (1<<UMSEL11)|(1<<UMSEL10)|(1<<UCPHA1)|(0<<UCPOL1);
/* Enable receiver and transmitter. */
UCSR1B = (1<<RXEN1)|(1<<TXEN1);
/* Set baud rate. */
/* IMPORTANT: The Baud Rate must be set after the transmitter is enabled
*/
UBRR1 = DEBUG_BAUD;



	/* Configure UART0 baud rate, one start bit, 8-bit, no parity and one stop bit 
	UBRR1H = (F_CPU/(DEBUG_BAUD*16L)-1) >> 8;
	UBRR1L = (F_CPU/(DEBUG_BAUD*16L)-1);
	UCSR1B = _BV(RXEN1) | _BV(TXEN1);
	UCSR1C = _BV(UCSZ10) | _BV(UCSZ11);
	*/
	/* Setup new streams for input and output 
	static FILE uout = FDEV_SETUP_STREAM(uputchar1, NULL, _FDEV_SETUP_WRITE);
	static FILE uin = FDEV_SETUP_STREAM(NULL, ugetchar1, _FDEV_SETUP_READ);
	*/
	/* Redirect all standard streams to UART0 
	stdout = &uout;
	stderr = &uout;
	stdin = &uin;
	*/
}
void init_adc(void)
{
	ADCSRA |= _BV(ADEN) | _BV(ADATE) | _BV(ADPS2) | _BV(ADPS1); /* enables ADC set bits to set pre-scaler to /64 */
	ADMUX = 0;
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