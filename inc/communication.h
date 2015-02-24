#ifndef COMMUNICATION_H
#define COMMUNICATION_H

// UART baudrate (750,000 bps)
#define BAUD		(F_CPU / 8 / (1 + 1))

// Response
#define COM_ACK		0
// End of variable length response
#define COM_END		0xFF

// Data structure: Type, Data
// No data, response {COM_ACK} indicates IlMatto2 exist
#define COM_PING	0
// Wakeup wireless module, response {COM_ACK}
#define COM_WAKEUP	1
// Suspend wireless module for power saving, response {COM_ACK}
#define COM_SUSPEND	2

// Wireless connection operations
// Ping for other end, no data, response {COM_ACK} for success, {COM_END} for timeout
#define COM_W_PING	100
// Start sending & receiving ADC data, no data, response {COM_ACK}
#define COM_W_ADC	101
// Send data to other end, data {Length(2 bytes), Data}, response {COM_ACK}
#define COM_W_SEND	102

#endif
