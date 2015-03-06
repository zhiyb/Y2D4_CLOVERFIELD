#include <avr/io.h>
#include <avr/interrupt.h>
#include "uart0.h"

#define READ()		UDR0
#define WRITE(d)	UDR0 = d
#define ENABLE_UDREI()	UCSR0B |= _BV(UDRIE0)
#define DISABLE_UDREI()	UCSR0B &= ~_BV(UDRIE0)
#define ENABLE_RXCI()	UCSR0B |= _BV(RXCIE0)
#define DISABLE_RXCI()	UCSR0B &= ~_BV(RXCIE0)

static uint8_t ack, pendingACK;
static volatile uint8_t ackRecv;

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
	UDR0;

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
	struct package_t *pkg = tx.buffer + current;
	if (!pkg->valid)
		return pkg;
	pkg = tx.buffer + (1 - current);
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
	if (ack) {
		WRITE(COM_ACK);
		ack = 0;
		return;
	}

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
	case UART0_TX_SENDING:
		goto disable;
	}

complete:
	tx.status = UART0_TX_SENDING;	// Waiting for ACK

disable:
	DISABLE_UDREI();
}

void uart0_received(void)
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
		pendingACK = 0;
		ENABLE_UDREI();
	}
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
	if (!pkg->valid) {
		ack++;
		ENABLE_UDREI();
	} else
		pendingACK++;
}
