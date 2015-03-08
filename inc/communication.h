#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#ifdef __cplusplus
extern "C" {
#endif

#include <inttypes.h>

// UART baudrate (750kbps)
#define BAUD		(F_CPU / 8 / (1 + 1))
// UART data buffer size
#define BUFFER_SIZE	64

// Response
#define COM_ACK		0
// End of variable length response / command failed
#define COM_END		0xFF

// From Il Matto 1

// Data masks
// Data type mask
#define COM_TYPE	0x0F
// Variable length data (length byte & data)
#define COM_DATA	0x80

// Data structure: Command, Data
// No data, response {COM_ACK} indicates IlMatto2 exist
#define COM_PING	1
// Wakeup wireless module
#define COM_WAKEUP	2
// Suspend wireless module for power saving
#define COM_SUSPEND	3

// Wireless connection operations
// Ping for other end, no data
#define COM_W_PING	4
// Ping success reply
#define COM_W_PING_SU	5
// Ping timeout reply
#define COM_W_PING_TO	6
// Timeout for remote ping, unit: ms
#define COM_W_PING_TIMEOUT	100
// Start sending & receiving sound data, no data
#define COM_W_SOUND	7
// Stop sending & receiving sound data, no data
#define COM_W_SOUND_END	8
// Send data to other end, data {Length(1 byte), Data}
#define COM_W_SEND	(COM_DATA | 20)

// Data received, data {Length(1 byte), Data}
#define COM_W_RECV	(COM_DATA | 21)

// Both send & receive can use the same buffering package type
struct package_t {
	volatile uint8_t valid;
	uint8_t command, length;
	uint8_t data[BUFFER_SIZE];
};

#ifdef __cplusplus
}
#endif

#endif
