#include <avr/io.h>
#include <util/delay.h>
#include <ctype.h>

int main(void){
	DDRD |= _BV(4); //clk
	DDRD |= _BV(3); //data
	DDRD |= _BV(2); //load
	
	
}