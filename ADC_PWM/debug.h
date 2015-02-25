/*  Author: Steve Gunn
 * Licence: This work is licensed under the Creative Commons Attribution License.
 *          View this license at http://creativecommons.org/about/licenses/
 *   Notes: F_CPU must be defined to match the clock frequency
 *   Usage: Include in your main file and call init_debug_uart0() from the beginning of main
 *          to initialise redirection of stdout, stderr and stdin to UART0.
 */

#define __ASSERT_USE_STDERR
#include <assert.h>
#include <stdio.h>
#include <avr/io.h>

#define DEBUG_BAUD  9600

void init_tone(void);
int uputchar0(char c, FILE *stream);
char get_ch();
int ugetchar0(FILE *stream);
void init_debug_uart0(void);
void init_adc(void);
void channel_adc(uint8_t n);
void adc_interrupt_enable(void);
void adc_start(void);
void adc_stop(void);