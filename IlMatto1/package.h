/*
 *  Author: Yubo Zhi (yz39g13@soton.ac.uk)
 */

#ifndef PACKAGE_H
#define PACKAGE_H

#define PKG_TYPE_SKETCH		0
#define PKG_TYPE_SKETCH_CLEAN	1
#define PKG_TYPE_REQUEST	2
#define PKG_TYPE_REQUEST_ACK	3

#define PKG_SKETCH_PREPEND	4

#define PKG_REQUEST_SKETCH	0

#define PKG_REQUEST_ACCEPT	0
#define PKG_REQUEST_REJECT	1
#define PKG_REQUEST_RECEIVED	2
#define PKG_REQUEST_CLOSED	3
#define PKG_REQUEST_INVALID	0xFF

extern const char *requestName[1];

union pkgSketch_t {
	uint8_t d[BUFFER_SIZE];
	struct {
		uint8_t type;
		uint8_t size;
		uint16_t clr;
		uint16_t pos[BUFFER_SIZE - PKG_SKETCH_PREPEND][2];
	} s;
};

union pkgRequest_t
{
	uint8_t d[2];
	struct {
		uint8_t type;
		uint8_t req;
	} s;
};

union pkgRequestAck_t
{
	uint8_t d[2];
	struct {
		uint8_t type;
		uint8_t req;
		uint8_t ack;
	} s;
};

#endif
