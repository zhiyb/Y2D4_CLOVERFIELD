/*
 *  Author: Yubo Zhi (yz39g13@soton.ac.uk)
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "tick.h"

static volatile uint16_t tk;

void tick_init(void)
{
	// Clock div 8, 1ms cycle
	TCCR1A = 0;
	TCCR1B = _BV(WGM12) | _BV(CS11);
	TCCR1C = 0;
	OCR1A = 1500;
	TIMSK1 = _BV(OCIE1A);
	TIFR1 |= _BV(OCF1A);

	tk = 0;
}

uint16_t tick(void)
{
	return tk;
}

ISR(TIMER1_COMPA_vect, ISR_NOBLOCK)
{
	if (++tk == TICK_CYCLE)
		tk = 0;
}
