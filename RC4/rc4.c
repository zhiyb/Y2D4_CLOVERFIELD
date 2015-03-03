#include <inttypes.h>
#include "rc4.h"

static uint8_t i, j;
static uint8_t s[256];

void rc4_regen(const char *key, const uint8_t length)
{
	i = 0;
	do
		s[i] = i;
	while (++i != 0);
	i = 0;
	j = 0;
	do {
		j = (j + s[i] + key[i % length]) % 256;
		uint8_t tmp = s[i];
		s[i] = s[j];
		s[j] = tmp;
	} while (++i != 0);
	i = 0;
	j = 0;
}

unsigned char rc4_output(void)
{
	i = (i + 1) % 256;
	j = (j + s[i]) % 256;
	uint8_t tmp = s[i];
	s[i] = s[j];
	s[j] = tmp;
	return s[(s[i] + s[j]) % 256];
}

void rc4_codec(char *buff, const uint8_t length)
{
	uint8_t l;
	for (l = length; l; l--, buff++)
		*buff ^= rc4_output();
}
