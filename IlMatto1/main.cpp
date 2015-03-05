/*
 * Author: Yubo Zhi (yz39g13@soton.ac.uk)
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <tft.h>
#include <portraitlist.h>
#include <rtouch.h>
#include <adc.h>
#include <eemem.h>
#include <colours.h>
#include <communication.h>
#include "uart0.h"
#include "menu.h"
#include "sketch.h"

static tft_t tft;
static rTouch touch(&tft);
static PortraitList l(&tft);
static sketch_t sketch(&tft, &touch);

void init(void)
{
	DDRB |= 0x80;			// LED
	PORTB |= 0x80;

	adc_init();
	adc_enable();
	uart0_init();

	tft.init();
	tft.setOrient(tft.Portrait);
	tft.setForeground(0x667F);
	tft.setBackground(0x0000);
	tft.clean();

	stdout = tftout(&tft);
	touch.init();
	menu::setTFT(&tft);
	menu::setTouch(&touch);
	menu::setSketch(&sketch);
	sei();

	tft.setBGLight(true);
	touch.calibrate();
	eeprom_first_done();
}

int main(void)
{
	init();

	tft.clean();
	tft.setForeground(0x0000);

	l.refresh();
	l.setRootItem(&menu::root::item);
	l.display(&menu::root::item);

	for (;;)
		l.pool(&touch);

	return 1;

#if 0
#if 0
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
#endif
}
