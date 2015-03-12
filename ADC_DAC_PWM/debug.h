#define __ASSERT_USE_STDERR
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#define DEBUG_BAUD 57600
int uputchar0(char c, FILE * stream);
int ugetchar0(FILE * stream);
void init_debug_uart0(void);
char get_ch();
