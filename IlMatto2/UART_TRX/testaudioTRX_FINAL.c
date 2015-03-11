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

//#define BAUD 57600

#define AUDIO 0xAA
#define IMAGE 0xCC
#define BUFFLEN 64

volatile uint8_t count = 0;
volatile uint8_t adc_buffer[BUFFLEN];
volatile uint8_t pwm_buffer[BUFFLEN];

volatile uint8_t audioTX = 0, audioRX = 0;

ISR(ADC_vect) {
  TIFR0 |= _BV(OCF0A); //reset compare match flag 
  if(count < BUFFLEN) {
    if(!audioTX) {
      pwm_set(pwm_buffer[count]);
    }
    adc_buffer[count] = ADCH;
    
    count++;
  }
}


uint8_t encryption(uint8_t data)
{
  int k = 3;

  uint8_t result;
  result = ((data<<k)|(data >>(8 - k)));
  return result;
}

uint8_t decryption(uint8_t data)
{
  int k = 3;

  uint8_t result;
  result = ((data>>k)|(data <<(8 - k)));
  return result;
}


int main() {

  
  _delay_ms(1000);
  rfm12_init();
  //init_debug_uart0();
  initTimer0();
  uart0_init();
    
  pwm_init();
  pwm_enable(1);
  initi_ADC();
  adc_interrupt_enable();
  
  sei();
  adc_start();
  _delay_ms(1000);

  DDRC &= ~_BV(PC0);

  uint8_t i;

  uint8_t data_type = 0;
  uint8_t transmit_buffer[BUFFLEN];
  uint8_t ilmatto1_data[BUFFLEN];
  uint8_t *receive_buffer = NULL;

  struct package_t *pkg;

  DDRC |= _BV(6);

  while(1) {

    /*if(count >= BUFFLEN) {
      count = 0;
    }*/

    pkg = uart0_rxPackage();

    if(pkg) {
      switch (pkg -> command) {
        case COM_PING:
          uart0_done(pkg);
          break;
        case COM_W_AUDIO_RX:
          audioRX = 1;
          uart0_done(pkg);
          break;
        case COM_W_AUDIO_TX:
          audioTX = 1;
          uart0_done(pkg);
          break;
	case COM_W_AUDIO_TX_END:
          audioTX = 0;
          uart0_done(pkg);
          break;
        case COM_FREQ:
          rfm12_set_frequency(((uint16_t)pkg->data[1] << 8) | ((uint16_t)pkg->data[0]));
          uart0_done(pkg);
          break;
        case COM_W_SEND:
          PORTC |= _BV(6);
          for(i = 0; i < BUFFLEN; i++) {
            ilmatto1_data[i] = encryption(pkg -> data[i]);
          }
          while (rfm12_tx ((pkg -> length), IMAGE, ilmatto1_data) == RFM12_TX_OCCUPIED)
            rfm12_tick();
          PORTC &= ~_BV(6);
          //norman wants ack from other end here
          uart0_done(pkg);
          break;
        default:
          uart0_done(pkg);
          break;

      }
    }

    if(audioTX) {//push button for now, can integrate the "talk" command when we add IL MATTO 1

      if(count >= BUFFLEN) { //executes when adc buffer is full
        //prepare ADC buffer for transmit 
        
        for(i=0; i<BUFFLEN; i++) {
          transmit_buffer[i] = encryption(adc_buffer[i]);   //encrypt the data
        }


        while (rfm12_tx(sizeof(transmit_buffer), AUDIO, transmit_buffer) != RFM12_TX_ENQUEUED)
          rfm12_tick();
    

        count = 0;

      }
    }
    if(rfm12_rx_status() == STATUS_COMPLETE) {//executes when data has been received

        receive_buffer = rfm12_rx_buffer();
        data_type = rfm12_rx_type();

      if(data_type == AUDIO) {
        //put data on pwm_buffer
        for(i=0; i<BUFFLEN; i++) {
          pwm_buffer[i] = decryption(receive_buffer[i]);    //encrypt
        }
        count = 0;

      }
      else if(data_type == IMAGE) {
        //send data to IL MATTO 1
        while (!(pkg = uart0_txPackage()));
        pkg->command = COM_W_RECV;
        pkg->length = rfm12_rx_len();
        for(i = 0; i < rfm12_rx_len(); i++) {
          pkg -> data[i] = decryption(receive_buffer[i]);
        }
        pkg->valid++;
        uart0_send();
      }
      rfm12_rx_clear();
    }
    rfm12_tick();
    /*
      this is where the code for integrating ILMATTO 1 will go
      it will take less of a priority than the code above
    */
  }
}
