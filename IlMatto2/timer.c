#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "timer.h"

void initTimer0(void) {
	TCCR0A = _BV(WGM01); //CTC MODE
	TCCR0B = _BV(CS01); // 1/8 clk

	OCR0A = 93;//(int)(12000000/(8000*2*8) - 1);
}

void startTimer0Int(void) {

	TIMSK0 |= _BV(OCIE0A); //interrupt on compare match A enable
}

void stopTimer0Int(void) {

	TIMSK0 &= ~_BV(OCIE0A); //interrupt on compare match A disable
}
