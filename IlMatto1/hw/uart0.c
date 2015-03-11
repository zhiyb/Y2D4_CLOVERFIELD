/*
 *  Author: Yubo Zhi (yz39g13@soton.ac.uk)
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "uart0.h"

// UART0 tx status masks
#define UART0_TX_SENDING	0x10	// Sending sequence of data
#define UART0_TX_STATUS		0x0F	// Status mask

// UART0 tx status
#define UART0_TX_IDLE		0			// Idle
#define UART0_TX_WAITING	1			// Waiting for ACK
#define UART0_TX_COMMAND	(UART0_TX_SENDING | 2)	// Sending command byte
#define UART0_TX_LENGTH		(UART0_TX_SENDING | 3)	// Sending length byte
#define UART0_TX_DATA		(UART0_TX_SENDING | 4)	// Sending data

// UART0 rx status masks
#define UART0_RX_RECEIVING	0x10	// Receiving sequence of data
#define UART0_RX_STATUS		0x0F	// Status mask

// UART0 rx status
#define UART0_RX_IDLE		0	// Idle
#define UART0_RX_COMMAND	(UART0_RX_RECEIVING | 1)	// Receiving command byte
#define UART0_RX_LENGTH		(UART0_RX_RECEIVING | 2)	// Receiving length byte
#define UART0_RX_DATA		(UART0_RX_RECEIVING | 3)	// Receiving data

#define READ()		UDR0
#define WRITE(d)	UDR0 = d
#define ENABLE_UDREI()	UCSR0B |= _BV(UDRIE0)
#define DISABLE_UDREI()	UCSR0B &= ~_BV(UDRIE0)
#define ENABLE_RXCI()	UCSR0B |= _BV(RXCIE0)
#define DISABLE_RXCI()	UCSR0B &= ~_BV(RXCIE0)

static uint8_t ack, pendingACK;
volatile static uint8_t ackRecv;

static struct buffer_t {
	struct package_t buffer[2];
	volatile uint8_t current;
	uint8_t status, pos;
} tx, rx;

void uart0_init(void)
{
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
	READ();

	// Data struct reset
	uart0_reset();

	// Interrupt
	ENABLE_RXCI();
}

void uart0_reset(void)
{
	DISABLE_UDREI();

	uint8_t i;
	for (i = 0; i < 2; i++)
		rx.buffer[i].valid = tx.buffer[i].valid = 0;
	tx.status = UART0_TX_IDLE;
	rx.status = UART0_RX_IDLE;
	rx.current = tx.current = 0;
	rx.pos = tx.pos = 0;
	ack = 0;
	ackRecv = 0;
	pendingACK = 0;
}

void uart0_send(void)
{
	ENABLE_UDREI();
}

struct package_t *uart0_txPackage(void)
{
	uint8_t current = tx.current;
	struct package_t *pkg = &tx.buffer[current];
	if (!pkg->valid)
		return pkg;
	pkg = &tx.buffer[1 - current];
	if (!pkg->valid)
		return pkg;
	return 0;
}

uint8_t uart0_ack(void)
{
	DISABLE_RXCI();
	uint8_t ack = ackRecv;
	ackRecv = ack ? ack - 1 : 0;
	ENABLE_RXCI();
	return ack;
}

ISR(USART0_UDRE_vect)
{
	uint8_t current = tx.current;
	struct package_t *pkg = tx.buffer + current;
	if (tx.status & UART0_TX_SENDING)
		goto sending;

	// ACK request?
	while (ack) {
		ack--;
		WRITE(COM_ACK);
	}

	if (tx.status == UART0_TX_WAITING)
		goto disable;

	// Current buffer valid?
	if (!pkg->valid)
		goto disable;

	// Send current buffer
	WRITE(pkg->command);
	if (!(pkg->command & COM_DATA))
		goto complete;
	tx.status = UART0_TX_COMMAND;
	return;

sending:
	switch (tx.status) {
	case UART0_TX_COMMAND:
		WRITE(pkg->length);
		if (pkg->length == 0)
			goto complete;
		tx.status = UART0_TX_LENGTH;
		tx.pos = 0;
		return;
	case UART0_TX_LENGTH:
	case UART0_TX_DATA:
		WRITE(pkg->data[tx.pos++]);
		if (tx.pos == pkg->length)
			goto complete;
		tx.status = UART0_TX_DATA;
		return;
	}

complete:
	tx.status = UART0_TX_WAITING;	// Waiting for ACK
	PORTB |= _BV(6);

disable:
	DISABLE_UDREI();
}

static void uart0_received(void)
{
	uint8_t current = rx.current;
	struct package_t *pkg = rx.buffer + current;
	if (pkg->valid) {
		current = 1 - current;
		pkg = rx.buffer + current;
		if (pkg->valid)
			return;
	}
	if (pendingACK) {
		ack++;
		pendingACK--;
		ENABLE_UDREI();
	}
}

void uart0_done(struct package_t *pkg)
{
	if (!pkg)
		return;
	pkg->valid = 0;
	uart0_received();
}

struct package_t *uart0_rxPackage(void)
{
	uint8_t current = 1 - rx.current;
	struct package_t *pkg = rx.buffer + current;
	if (pkg->valid)
		return pkg;
	pkg = rx.buffer + (1 - current);
	if (pkg->valid)
		return pkg;
	return 0;
}

ISR(USART0_RX_vect)
{
	uint8_t current = rx.current;
	struct package_t *pkg = rx.buffer + current;
	if (rx.status & UART0_RX_RECEIVING)
		goto receiving;

	uint8_t c = READ();
	if (c == COM_ACK) {
		PORTB &= ~_BV(6);
		current = tx.current;
		(tx.buffer + current)->valid = 0;
		tx.current = 1 - current;
		tx.status = UART0_TX_IDLE;
		ENABLE_UDREI();
		ackRecv++;
		return;
	}

	pkg->command = c;
	if (!(c & COM_DATA))
		goto complete;
	rx.status = UART0_RX_COMMAND;
	return;

receiving:
	c = READ();
	switch (rx.status) {
	case UART0_RX_COMMAND:
		pkg->length = c;
		if (pkg->length == 0)
			goto complete;
		rx.status = UART0_RX_LENGTH;
		rx.pos = 0;
		return;
	case UART0_RX_LENGTH:
	case UART0_RX_DATA:
		pkg->data[rx.pos++] = c;
		if (rx.pos == pkg->length)
			goto complete;
		rx.status = UART0_RX_DATA;
		return;
	}

complete:
	rx.status = UART0_RX_IDLE;
	pkg->valid = 1;
	rx.current = current = 1 - current;

	pkg = rx.buffer + current;
	if (!pkg->valid) {	// Can receive another package
		ack++;
		ENABLE_UDREI();
	} else
		pendingACK++;
}
