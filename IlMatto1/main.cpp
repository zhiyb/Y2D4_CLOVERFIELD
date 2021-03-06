/*
 * Author: Yubo Zhi (yz39g13@soton.ac.uk)
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <adc.h>
#include <eemem.h>
#include "menu.h"
#include "pool.h"
#include "common.h"
#include "capture.h"

using namespace colours::b16;

tft_t tft;
rTouch touch(&tft);
PortraitList list(&tft);
sketch_t sketch;
keypad_t keypad;
status_t status;
pin_t pin;
notification_t notification;

void init(void)
{
	DDRB |= _BV(7);
	PORTB |= _BV(7);		// LED

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
	capture::init(&tft);
	capture::enable();
	notification.init();
	touch.init();
	keypad.init();
	sei();

	tft.setBGLight(true);
	touch.calibrate();
	keypad.calibrate();
	if (!pin.init())
		pool::pinSet();
	else
		pool::pinLock();
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

	for (;;)
		pool::list();

	return 1;
}
