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

// From Il Matto 1

// Data masks
// Data type mask
#define COM_TYPE	0x0F
// Variable length data (length byte & data)
#define COM_DATA	0x80

// Data structure: Command [length data]
// Command without COM_DATA mask indicates no length & data
// Command with COM_DATA mask can have data with length <= BUFFER_SIZE
// Acknowledge COM_ACK after entire command sequence

// General operations
// No data, response {COM_ACK} indicates IlMatto2 exist
#define COM_PING	1
// Wakeup wireless module
#define COM_WAKEUP	2
// Suspend wireless module for power saving
#define COM_SUSPEND	3
// Set wireless module frequency, data 2 bytes, LE uint16_t
#define COM_FREQ	4

// Wireless connection operations
// Ping for other end
#define COM_W_PING	5
// Ping succeed reply
#define COM_W_PING_SU	6
// Ping timeout reply
#define COM_W_PING_TO	7
// Suggested timeout for remote ping, unit: ms
#define COM_W_PING_TIMEOUT	100

// Start transmiting audio data
#define COM_W_AUDIO_TX	8
// Start receiving audio data
#define COM_W_AUDIO_RX	9
// Stop sending & receiving audio data
#define COM_W_AUDIO_END	10
// Send data to other end
#define COM_W_SEND	(COM_DATA | 11)
// Data received
#define COM_W_RECV	(COM_DATA | 11)

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
