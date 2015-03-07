#ifndef KEYPAD_H
#define KEYPAD_H

// Horizontal size
#define KEYPAD_SIZE_X	4
// Vertical size
#define KEYPAD_SIZE_Y	4
// Index size
#define KEYPAD_SIZE	(KEYPAD_SIZE_X * KEYPAD_SIZE_Y)

// Keypad default display colour
#define KEYPAD_DEF_CLR	colours::b16::Blue
// Keypad activative display colour
#define KEYPAD_ACT_CLR	colours::b16::Red

// Key code for N/A
#define KEYPAD_NA	0xFF

#include <avr/pgmspace.h>
#include <rtouch.h>
#include <colours.h>

class keypad_t
{
public:
	void init(void);
	void calibrate(bool reset = false);
	void recalibrate(void) {calibrate(true);}
	void display(void) const;
	// Arg: keep return pressing key, Ret: index of pressed key or KEYPAD_NA
	uint8_t pool(bool keep = false);
	bool testPool(void);

private:
	// Convert index to coordinate relative to TOP-LEFT for DISPLAY
	rTouch::coord_t coordinate(uint8_t idx) const;
	void drawCross(const rTouch::coord_t pos, uint16_t c) const;
	void drawKey(uint8_t idx, uint16_t clr = KEYPAD_DEF_CLR) const;
	void drawKeypad(void) const;
	uint8_t keyAt(const rTouch::coord_t pos) const;

	uint8_t prev, prevTest;
	struct cal_t {
		rTouch::coord_t pos, size;
	} cal;
	static struct cal_t EEMEM NVcal;
	static const char PGMkeyName[KEYPAD_SIZE] PROGMEM;
};

#endif
