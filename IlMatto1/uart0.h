#ifndef UART0_H
#define UART0_H

#include <stdio.h>

namespace uart0
{
	FILE *init(void);
	int getchNonBlocking(void);
	char getch(void);
	void putch(const char c);
	void poolSending(void);
}

#endif
