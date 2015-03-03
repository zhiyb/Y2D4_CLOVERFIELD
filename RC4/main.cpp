#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <communication.h>
#include <tft.h>
#include "rc4.h"

class tft_t tft;

void init(void)
{
	DDRB |= 0x80;			// LED
	PORTB |= 0x80;
	tft.init();
	tft.setOrient(tft.Portrait);
	tft.setForeground(0x667F);
	tft.setBackground(0x0000);
	tft.clean();
	stdout = tftout(&tft);
	tft.setBGLight(true);
}

int main(void)
{
	init();

start:
	tft.clean();
	tft.setZoom(1);
	puts("*** RC4 testing ***");

	char string[] = "D4 Cloverfield!";
	char key[] = "The secret RC4 key";
	printf("Original: %s\n", string);
	rc4_regen(key, sizeof(key));
	rc4_codec(string, sizeof(string));
	puts("Encrypted:");
	for (uint8_t i = 0; i < sizeof(string); i++)
		printf("%d, ", string[i]);
	rc4_regen(key, sizeof(key));
	rc4_codec(string, sizeof(string));
	printf("\nDecrypted: %s\n", string);

loop:
	goto loop;
	goto start;

	return 1;
}
