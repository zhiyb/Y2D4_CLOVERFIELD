#ifndef PIN_H_
#define PIN_H_

#include <communication.h>
#include <eemem.h>

#define PIN_LENGTH	4
//#define PIN_DISPLAY

class pin_t
{
public:
	bool init(void);
	package_t *pool(package_t *pkg);
	void set(uint16_t v);
	bool done(void) const {return pos == PIN_LENGTH;}
	bool matched(void) const {return input() == pin;}
	uint16_t input(void) const;
	void restart(void);
	void display(const char *str);

private:
	void put(uint8_t c);
	void remove(void);

	uint8_t pos, ip[PIN_LENGTH];
	uint16_t pin;
	static uint16_t EEMEM NVpin;
};

#endif
