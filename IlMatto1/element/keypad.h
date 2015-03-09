/*
 *  Author: Yubo Zhi (yz39g13@soton.ac.uk)
 */

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
// Key code for delete
#define KEYPAD_DEL	127

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
	uint8_t pool(bool keep = false, bool code = true);
	bool testPool(void);
	bool colourPicker(rTouch::coord_t pos, uint16_t &clr) const;
	bool outside(const rTouch::coord_t pos) const;
	bool outsideLeft(const int16_t x) const {return x < cal.pos.x;}
	uint8_t keyAt(const rTouch::coord_t pos) const;
	// Translate key index to key code
	uint8_t translate(uint8_t idx);
	void initText(void) {prev = 0;}
	char text(void);

private:
	// Convert index to coordinate relative to TOP-LEFT for DISPLAY
	rTouch::coord_t coordinate(uint8_t idx) const;
	void drawCross(const rTouch::coord_t pos, uint16_t c) const;
	void drawKey(uint8_t idx, uint16_t clr = KEYPAD_DEF_CLR) const;
	void drawKeypad(void) const;

	uint8_t prev, prevTest, index, status;
	rTouch::coord_t first, prevCoord;
	struct cal_t {
		rTouch::coord_t pos, size;
	} cal;
	static struct cal_t EEMEM NVcal;

	// Display name (1 char)
	static const char PGMkeyName[KEYPAD_SIZE] PROGMEM;
	// Key code
	static const uint8_t PGMkeyCode[KEYPAD_SIZE] PROGMEM;
	// Text
	static const char PGMkeyText[KEYPAD_SIZE - 1][5] PROGMEM;
};

#endif
