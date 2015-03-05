/*
 * Author: Yubo Zhi (yz39g13@soton.ac.uk)
 */

#ifndef LIST_H
#define LIST_H

// Size of image on each items
#ifndef ITEM_IMAGE_SIZE
#define ITEM_IMAGE_SIZE	(FONT_HEIGHT * 2)
#endif

// Sub menus stack level
#ifndef ITEM_STACK_SIZE
#define ITEM_STACK_SIZE	5
#endif

#include <avr/pgmspace.h>
#include <inttypes.h>

struct listItem {
	// Display name of this item
	const char *name;
	// Icon of this item
	const uint8_t *image;
	// Children items (sub menu)
	const listItem **items;
	// Function to execute, return value: Enter next level or not
	bool (*func)(void);
};

#endif
