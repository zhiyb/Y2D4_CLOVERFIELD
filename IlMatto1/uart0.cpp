#include <avr/io.h>
#include <stdio.h>
#include "uart0.h"
#include "communication.h"

void uart0::poolSending(void)
{
	while (!(UCSR0A & _BV(UDRE0)));
}

void uart0::putch(const char c)
{
	//if (ch == '\n')
	//	putchar('\r');
	poolSending();
	UDR0 = c;
}

static int putch(char ch, FILE *stream)
{
	uart0::putch(ch);
	return ch;
}

int uart0::getchNonBlocking(void)
{
	if (!(UCSR0A & (1<<RXC0)))
		return -1;
	return UDR0;
}

char uart0::getch(void)
{
	while (!(UCSR0A & (1<<RXC0)));
	return UDR0;
}

static int getch(FILE *stream)
{
	return uart0::getch();
}

FILE *uart0::init(void)
{
	#include <util/setbaud.h>
	DDRD &= ~0x03;
	PORTD |= 0x03;
	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;
	UCSR0A = USE_2X << U2X0;
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	UCSR0C = (1 << UCSZ00) | (1 << UCSZ01);
	return fdevopen(::putch, ::getch);
}
