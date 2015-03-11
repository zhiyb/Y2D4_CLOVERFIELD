#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include <ctype.h>

#include "rfm12.h"
//#include "debug.h"
#include "pwm.h"
#include "timer.h"
#include "adc.h"
#include "uart0.h"
#include "communication.h"

#define AUDIO 0xAA
#define IMAGE 0xCC
#define BUFFLEN 64

//#define BAUD 57600

volatile uint8_t count = 0;
volatile uint8_t adc_value = 0;

	

ISR(ADC_vect)
{
    TIFR0 |= _BV(OCF0A); //reset compare match flag 
    adc_value = ADCH;
    if(count < BUFFLEN) {
    	count++;
    }
}

int main() {

	rfm12_init();
	//init_debug_uart0();
	uart0_init();

  	initTimer0();
  	
  	pwm_init();
  	pwm_enable(1);
  	//startTimer0Int();
	initi_ADC(); 
	sei();
	adc_interrupt_enable();
	adc_start();

	DDRC &= ~_BV(PC0);


	uint8_t *receive_buffer = NULL;
	uint8_t pwm_buffer[BUFFLEN];
	uint8_t transmit_buffer[BUFFLEN];

	uint8_t transmit = 0, type;

	struct package_t *pkg;

	//uint8_t i;
	//uint8_t mode = 0; //0 = RX , 1 = TX

	while(1) {
		//TXMode(&mode);

		pkg = uart0_rxPackage();

		if(pkg) {
			switch (pkg -> command) {
				case COM_PING:
					uart0_done(pkg);
					break;
				case COM_W_AUDIO:
					transmit = 1;
					uart0_done(pkg);
					break;
				case COM_W_AUDIO_END:
					transmit = 0;
					uart0_done(pkg);
					break;
				case COM_W_SEND:
					while (rfm12_tx ((pkg -> length), IMAGE, (pkg -> data)) != RFM12_TX_ENQUEUED)
    					rfm12_tick();
					//norman wants ack from other end here
					uart0_done(pkg);
					break;

			}
		}


		if(transmit) {
			
    		if(count < BUFFLEN) {        
      			transmit_buffer[count] = adc_value;//fill transmit buffer 1
    		}

    		if(count >= BUFFLEN) {
      			while (rfm12_tx (sizeof(transmit_buffer), AUDIO, transmit_buffer) != RFM12_TX_ENQUEUED)
    				rfm12_tick();
      			count = 0;
    		}
    	}
    	rfm12_tick();

  		if (rfm12_rx_status() == STATUS_COMPLETE) {
			receive_buffer = rfm12_rx_buffer();
			type = rfm12_rx_type();
			if(type == IMAGE) {
				while (!(pkg = uart0_txPackage()));
				pkg->command = COM_W_RECV;
				pkg->length = rfm12_rx_len();
				memcpy(pkg->data, receive_buffer, rfm12_rx_len());
				pkg->valid++;
				uart0_send();
			} else {
				memcpy ( pwm_buffer, receive_buffer, BUFFLEN );
			}
			count = 0;
			rfm12_rx_clear();
		}

		if(count < BUFFLEN) {
			pwm_set(pwm_buffer[count]);
		}
		else {
			pwm_set(0);
		}
 	}

}