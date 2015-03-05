#ifndef UART0_H
#define UART0_H

#ifdef __cplusplus
extern "C" {
#endif

#include <communication.h>

// UART0 tx status
#define UART0_TX_STATUS		0x0F	// Status mask
#define UART0_TX_IDLE		0	// Idle
#define UART0_TX_COMMAND	1	// Sending command byte, waiting for ACK
#define UART0_TX_LENGTH		2	// Sending length byte
#define UART0_TX_DATA		3	// Sending data
#define UART0_TX_WAITING	4	// Waiting for ACK after data
// UART0 tx status masks
#define UART0_TX_END		0x10	// End of data received

// UART0 rx status
#define UART0_RX_STATUS		0x0F	// Status mask
#define UART0_RX_IDLE		0	// Idle
#define UART0_RX_COMMAND	1	// Receiving command byte, waiting for send ACK
#define UART0_RX_LENGTH		2	// Receiving length byte
#define UART0_RX_DATA		3	// Receiving data
// UART0 tx status masks
#define UART0_RX_END		0x10	// End of data received
#define UART0_RX_WAITING	0x20	// Waiting for send ACK
#define UART0_RX_VARSIZE	0x40	// Variable size data

// Initialisation
void uart0_init(void);
// Request available tx package, 0 for not available
struct package_t *uart0_txPackage(void);
// Send the available tx package requested from above function
void uart0_send(void);
// Tx status
uint8_t uart0_txStatus(void);
// Re-send current tx package
void uart0_txRetry(void);
// Abort current tx package (mark as invalid, continue to next)
void uart0_txAbort(void);

#ifdef __cplusplus
}
#endif

#endif
