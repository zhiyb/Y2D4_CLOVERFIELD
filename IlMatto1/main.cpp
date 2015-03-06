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
#include "tick.h"
#include "menu.h"
#include "sketch.h"
#include "pool.h"

using namespace colours::b16;

tft_t tft;
rTouch touch(&tft);
PortraitList list(&tft);
sketch_t sketch;

void init(void)
{
	DDRB |= 0x80;			// LED
	PORTB |= 0x80;

	adc_init();
	adc_enable();
	uart0_init();
	tick_init();

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
	tft.setForeground(Black);

	list.refresh();
	list.setRootItem(&menu::root::item);
	list.display(&menu::root::item);

	for (;;) {
		pool();
		list.pool(&touch);
	}

	return 1;
}
