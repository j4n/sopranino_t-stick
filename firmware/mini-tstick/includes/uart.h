// Copyright 2010 Jan Anlauff <jan at 23t.de>
/*
 * usart.h
 * Interrupt-driven code from http://www.rn-wissen.de/index.php/UART_mit_avr-gcc
 */

#ifndef USART_H_
#define USART_H_

#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "fifo.h"
#include "io.h"

#define BAUDRATE 57600UL

// Compatibility Aliases for ATMega168 and friends
#define    UCSRA    UCSR0A
#define    UCSRB    UCSR0B
#define    UCSRC    UCSR0C
#define    UBRRH    UBRR0H
#define    UBRRL    UBRR0L
#define    UDRE     UDRE0
#define    UDR      UDR0
#define    RXC      RXC0
#define    TXC      TXC0
#define    RXCIE    RXCIE0
#define    RXEN     RXEN0
#define    TXEN     TXEN0
#define    UCSZ0    UCSZ00
#define    UCSZ1    UCSZ01
#define    UDRIE    UDRIE0
// ugly fix-code- really fix when it works
#define    SIG_UART_TRANS    SIG_USART_TRANS
#define    SIG_UART_RECV     SIG_USART_RECV
#define    SIG_UART_DATA     SIG_USART_DATA

// old
//void uart_init(unsigned int);
//void uart_transfer(unsigned char);


extern void uart_init(void);
extern int uart_putc (const uint8_t);
int uart_putcs (const uint8_t, FILE *stream);
extern uint8_t uart_getc_wait(void);
extern int uart_getc_nowait(void);
void uart_puts (const char *s);
void uart_puts_P (PGM_P s);

static inline void uart_flush(void);
static inline void uart_flush() {
	while (UCSRB & (1 << UDRIE));
}

#endif /* USART_H_ */
