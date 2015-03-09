// http://www.avrfreaks.net/forum/avr-c-micro-how?page=all

#include <stdlib.h>

void* operator new(size_t size)
{
	return malloc(size);
}

void operator delete(void *ptr)
{
	free(ptr);
}
