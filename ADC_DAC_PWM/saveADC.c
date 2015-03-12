#include <util/delay.h>
#include "dac.h"
#include <avr/io.h>
#include "debug.h"
#include <ctype.h>
//#include "sound.h"

//volatile uint8_t val=0;
int main(void)
{
	volatile uint8_t data[30000];
	init_dac();
	uint16_t i = 0;
	initi_ADC();
	adc_start();
	for (i = 0; i < 30000 - 1; i++) {
		data[i] = ADCH;
	}
	while (1) {

		set_dac(data[i]);
		i++;
		if (i == 50000) {
			i = 0;
		};
		//for(i=0;i<26240;i++){
		//set_dac(sound_data[i]);
		//}
	}
	return 1;
}

/*ISR(ADC_vect)
	{
		//val = ADCH;
		//set_dac(ADCH);
	}
	*/
