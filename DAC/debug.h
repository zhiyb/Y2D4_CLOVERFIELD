#define __ASSERT_USE_STDERR
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define DEBUG_BAUD 115200
void init_tone(void);
int uputchar0(char c, FILE *stream);
int ugetchar0(FILE *stream);
void init_debug_uart0(void);
char get_ch();
void init_adc(void);
void channel_adc(uint8_t n);
void adc_interrupt_enable(void);
void adc_start(void);
void adc_stop(void);
void initi_ADC(void);