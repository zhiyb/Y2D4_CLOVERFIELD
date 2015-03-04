#include <avr/io.h>
#include <avr/interrupt.h>
#include "uart0.h"

#define READ()		UDR0
#define WRITE(d)	UDR0 = d
#define ENABLE_UDREI()	UCSR0B |= _BV(UDRIE0)
#define DISABLE_UDREI()	UCSR0B &= ~_BV(UDRIE0)
#define ENABLE_RXCI()	UCSR0B |= _BV(RXCIE0)
#define DISABLE_RXCI()	UCSR0B &= ~_BV(RXCIE0)

static struct buffer_t {
	struct package_t buffer[2];
	volatile uint8_t current, status;
	uint8_t pos;
} tx, rx;

void uart0_init(void)
{
	uint8_t i;
	// Initialise data structre
	for (i = 0; i < 1; i++)
		rx.buffer[i].valid = tx.buffer[i].valid = 0;
	rx.status = tx.status = UART0_TX_IDLE;
	rx.current = tx.current = 0;
	rx.pos = tx.pos = 0;

	// Initialise UART0_TX
	#include <util/setbaud.h>
	DDRD &= ~0x03;
	PORTD |= 0x03;
	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;
	UCSR0A = USE_2X << U2X0;
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	UCSR0C = (1 << UCSZ00) | (1 << UCSZ01);

	// Clear flags
	UCSR0A |= _BV(TXC0);
	UDR0;

	// Enable interrupt
	ENABLE_RXCI();
}

struct package_t *uart0_txPackage(void)
{
	uint8_t current = tx.status == UART0_TX_IDLE ? tx.current : 1 - tx.current;
	struct package_t *pkg = &tx.buffer[current];
	return pkg->valid ? 0 : pkg;
}

void uart0_send(void)
{
	ENABLE_UDREI();
}

uint8_t uart0_txStatus(void)
{
	return tx.status;
}

void uart0_txRetry(void)
{
	DISABLE_UDREI();
	tx.status = UART0_TX_IDLE;
	uart0_send();
}

void uart0_txAbort(void)
{
	DISABLE_UDREI();
	uint8_t current = tx.current;
	tx.buffer[current].valid = 0;
	tx.status = UART0_TX_IDLE;
	tx.current = 1 - current;
	if (tx.buffer[current].valid)
		ENABLE_UDREI();
}

ISR(USART0_RX_vect)
{
	PINB |= _BV(7);
	char c = READ();
	switch (c) {
	case COM_ACK:
		if (tx.status != UART0_TX_IDLE)
			ENABLE_UDREI();			// ACK received
		return;
	}
}

ISR(USART0_UDRE_vect)
{
	uint8_t current = tx.current;
	struct package_t *pkg = &tx.buffer[current];
	if (!pkg->valid) {
		DISABLE_UDREI();
		return;
	}
	switch (tx.status) {
	case UART0_TX_IDLE:
		WRITE(pkg->command);
		tx.status = UART0_TX_COMMAND;
		DISABLE_UDREI();			// Waiting for ACK
		return;
	case UART0_TX_COMMAND:
		if (pkg->length) {
			WRITE(pkg->length);
			tx.status = UART0_TX_LENGTH;
			tx.pos = 0;
		} else {
			pkg->valid = 0;
			tx.status = UART0_TX_IDLE;	// No data to transmit
			tx.current = 1 - current;
		}
		return;
	case UART0_TX_LENGTH:
	case UART0_TX_DATA:
		WRITE(pkg->data[tx.pos++]);
		tx.status = UART0_TX_DATA;
		if (tx.pos == pkg->length) {
			tx.status = UART0_TX_WAITING;
			DISABLE_UDREI();
		}
		return;
	case UART0_TX_WAITING:
		pkg->valid = 0;
		tx.status = UART0_TX_IDLE;
		tx.current = 1 - current;
		return;
	}
}
