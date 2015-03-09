/*
 *  Author: Yubo Zhi (yz39g13@soton.ac.uk)
 */

#ifndef POOL_H
#define POOL_H

#include <communication.h>

namespace pool
{
	// Package fetch & basic pool
	package_t *pool(void);

	void pinLock(void);
	void pinSet(void);
	void sketch(bool shared);
	void testKeypad(void);
	bool textInput(const char *str, char *buf);
	void list(void);
	void request(uint8_t req);
	bool sendRequest(uint8_t req);
	void audio(void);
	void message(const char *str);
	void sendMessage(const char *str);
	void tictactoe(void);
}

#endif
