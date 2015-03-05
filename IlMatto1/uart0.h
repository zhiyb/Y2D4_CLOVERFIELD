#ifndef UART0_H
#define UART0_H

#ifdef __cplusplus
extern "C" {
#endif

#include <communication.h>

// UART0 tx status masks
#define UART0_TX_SENDING	0x10	// Sending sequence of data
#define UART0_TX_STATUS		0x0F	// Status mask

// UART0 tx status
#define UART0_TX_IDLE		0			// Idle
#define UART0_TX_COMMAND	(UART0_TX_SENDING | 1)	// Sending command byte
#define UART0_TX_LENGTH		(UART0_TX_SENDING | 2)	// Sending length byte
#define UART0_TX_DATA		(UART0_TX_SENDING | 3)	// Sending data

// UART0 rx status masks
#define UART0_RX_RECEIVING	0x10	// Receiving sequence of data
#define UART0_RX_STATUS		0x0F	// Status mask

// UART0 rx status
#define UART0_RX_IDLE		0	// Idle
#define UART0_RX_COMMAND	(UART0_RX_RECEIVING | 1)	// Receiving command byte
#define UART0_RX_LENGTH		(UART0_RX_RECEIVING | 2)	// Receiving length byte
#define UART0_RX_DATA		(UART0_RX_RECEIVING | 3)	// Receiving data

// Initialisation
void uart0_init(void);
// Reset tx & rx
void uart0_reset(void);

// Send valid tx packages
void uart0_send(void);
// Request available tx package for write, 0 for not available
struct package_t *uart0_txPackage(void);
// ACK received
uint8_t uart0_ack(void);

// Done with rx package
void uart0_received(void);
// Check available rx package for read, 0 for none
struct package_t *uart0_rxPackage(void);

#ifdef __cplusplus
}
#endif

#endif
