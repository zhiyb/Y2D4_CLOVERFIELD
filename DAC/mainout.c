#include <util/delay.h>
#include "dac.h"
#include <avr/io.h>
#include "debug.h"
#include <ctype.h>

int main(void)
{
	init_dac();
	init_debug_uart0();
	init_adc(); 
	adc_start();
	char ch;
	while(1){
		ch = get_ch();
		//set_dac(ADCH);
		set_dac(ch);
		//printf("%d",ADCH);
	}
	
/* inc:
	set_dac(dac);
	_delay_ms(5);
	if (++dac != 255)
		goto inc;
dec:
	set_dac(dac);
	_delay_ms(5);
	if (--dac != 0)
		goto dec;
	goto inc;
*/
	return 1;
}
