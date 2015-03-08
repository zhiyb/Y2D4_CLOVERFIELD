#ifndef UART0_H
#define UART0_H

#ifdef __cplusplus
extern "C" {
#endif

#include <communication.h>

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
void uart0_done(struct package_t *pkg);
// Check available rx package for read, 0 for none
struct package_t *uart0_rxPackage(void);

#ifdef __cplusplus
}
#endif

#endif
