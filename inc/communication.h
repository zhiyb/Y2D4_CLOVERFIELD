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
// End of variable length response
#define COM_END		0xFF

// From Il Matto 1

// Data structure: Command, Data
// No data, response {COM_ACK} indicates IlMatto2 exist
#define COM_PING	50
// Wakeup wireless module, response {COM_ACK}
#define COM_WAKEUP	51
// Suspend wireless module for power saving, response {COM_ACK}
#define COM_SUSPEND	52

// Wireless connection operations
// Ping for other end, no data, response {COM_ACK} for success, {COM_END} for timeout
#define COM_W_PING	100
// Start sending & receiving sound data, no data, response {COM_ACK}
#define COM_W_SOUND	101
// Stop sending & receiving sound data, no data, response {COM_ACK}
#define COM_W_SOUND_END	102
// Send data to other end, data {Length(1 byte), Data}, response {COM_ACK}
#define COM_W_SEND	103

// Data received, data {Length(1 byte), Data}, response {COM_ACK}
#define COM_W_RECV	150

// Both send & receive can use the same buffering package type
struct package_t {
	uint8_t command, length, valid;
	uint8_t data[BUFFER_SIZE];
};

#ifdef __cplusplus
}
#endif

#endif
