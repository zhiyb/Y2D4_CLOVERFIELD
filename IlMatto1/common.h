/*
 *  Author: Yubo Zhi (yz39g13@soton.ac.uk)
 */

#ifndef COMMON_H
#define COMMON_H

#include <tft.h>
#include <colours.h>
#include <rtouch.h>
#include <portraitlist.h>
#include <communication.h>
#include "sketch.h"
#include "keypad.h"
#include "status.h"
#include "indicator.h"
#include "pin.h"
#include "uart0.h"
#include "tick.h"
#include "package.h"

#define TICK_CLEAR		0
#define TICK_PING		10
#define TICK_PING_CHECK		15
#define TICK_PING_REMOTE	10
#define TICK_PING_REMOTE_CHECK	(TICK_PING_REMOTE + COM_W_PING_TIMEOUT + 10)

using namespace colours::b16;

extern tft_t tft;
extern rTouch touch;
extern PortraitList list;
extern sketch_t sketch;
extern keypad_t keypad;
extern status_t status;
extern pin_t pin;

#endif
