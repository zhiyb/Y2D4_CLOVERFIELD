#ifndef COMMON_H
#define COMMON_H

#include <tft.h>
#include <colours.h>
#include <rtouch.h>
#include <portraitlist.h>
#include <communication.h>
#include "sketch.h"
#include "keypad.h"
#include "uart0.h"
#include "tick.h"

using namespace colours::b16;

extern tft_t tft;
extern rTouch touch;
extern PortraitList list;
extern sketch_t sketch;
extern keypad_t keypad;

#endif
