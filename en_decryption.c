/*
 * en_decryption.c
 *
 *  Created on: 7 Mar 2015
 *      Author: ak12g12
	idea of wrapped shifting from
	http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n3990.pdf
 */

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
