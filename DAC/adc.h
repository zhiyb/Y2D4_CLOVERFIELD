s#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

void init_adc(void);
void channel_adc(uint8_t n);
void adc_interrupt_enable(void);
void adc_start(void);
void adc_stop(void);
void initi_ADC(void);