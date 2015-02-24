#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <communication.h>
#include "tft.h"
#include "uart0.h"

class tft_t tft;
// UART0
FILE *u0;

void init(void)
{
	DDRB |= 0x80;			// LED
	PORTB |= 0x80;
	u0 = uart0::init();
	tft.init();
	tft.setOrient(tft.Portrait);
	tft.setForeground(0x667F);
	tft.setBackground(0x0000);
	tft.clean();
	stdout = tftout(&tft);
	tft.setBGLight(true);
}

int main(void)
{
	init();

start:
	tft.clean();
	tft.setZoom(1);
	puts("*** IlMatto1 testing ***");
	puts("PING for IlMatto2");
	char c;

ping:
	do {
		uart0::putch(COM_PING);
		uart0::poolSending();
		_delay_ms(10);
	} while ((c = uart0::getchNonBlocking()) == -1);
	if (c != COM_ACK)
		printf("Unknown: %u\n", c);
	goto ping;

	puts("Wakeup wireless module");
	uart0::putch(COM_WAKEUP);
	if ((c = uart0::getch()) != COM_ACK)
		printf("Unknown: %u\n", c);

	puts("Ping for other end");
	uart0::putch(COM_W_PING);
	switch ((uint8_t)(c = uart0::getch())) {
	case COM_ACK:
		puts("Responsed.");
		break;
	case COM_END:
		puts("Timeout.");
		break;
	default:
		printf("Unknown: %u\n", c);
		break;
	}

	puts("Start sound transmit");
	uart0::putch(COM_W_SOUND);
	if ((c = uart0::getch()) != COM_ACK)
		printf("Unknown: %u\n", c);

	puts("Finished");

loop:
	goto loop;
	goto start;

	return 1;
}
