#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>

#include "rfm12.h"


int main ( void )
{
	uint8_t *bufcontents;
	uint8_t i;
	uint8_t tv[] = "foobar";

	uint16_t ticker = 0;

	
	DDRA |= _BV(PA0);
	PORTA &= !_BV(PA0);

	_delay_ms(250);
	_delay_ms(250);
	_delay_ms(250);
	rfm12_init();
	
	sei();


	while (42)
	{
		ticker++;

		if (rfm12_rx_status() == STATUS_COMPLETE)
		{

			bufcontents = rfm12_rx_buffer();

			PORTA &= !_BV(PA0);8
			// tell the implementation that the buffer
			// can be reused for the next data.
			rfm12_rx_clear();
			_delay_ms(1000);

		}


//		if (!(PINB & (_BV(PB0))) && (ticker % 3000 == 0))
//		{
//			uart_putc ('#');
			rfm12_tx (sizeof(tv), 0, tv);
			
			PORTA |= _BV(PA0);
//		} 

		rfm12_tick();
	}
}
