#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
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
	uart0_init();
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
	sei();

start:
	tft.clean();
	tft.setZoom(1);
	puts("*** IlMatto1 testing ***");
	struct package_t *pkg;

	puts("PING for IlMatto2");
	while (!(pkg = uart0_txPackage()));
	pkg->command = COM_PING;
	pkg->length = 0;
	pkg->valid++;
	uart0_send();
	while (uart0_txStatus() != UART0_TX_IDLE);
	puts("Valid response received!");

#if 0
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
#endif

loop:
	goto loop;
	goto start;

	return 1;
}
