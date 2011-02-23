// Copyright 2010 Jan Anlauff <jan at 23t.de>
// adapted from http://www.rn-wissen.de/index.php/UART_mit_avr-gcc

#include "includes/uart.h"

//void uart_init(unsigned int bittimer) {
//	/* Set the baud rate */
//	UBRR0H = (unsigned char) (bittimer >> 8);
//	UBRR0L = (unsigned char) bittimer;
//	/* set the framing to 8N1 */
//	UCSR0C = (3 << UCSZ00);
//	/* Engage! */
//	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
//	return;
//}
//
//void uart_transfer(unsigned char c) {
//	while ( !(UCSR0A & (1 << UDRE0)) )
//		;
//	UDR0 = c;
//}

// FIFO-Objekte und Puffer für die Ein- und Ausgabe

#define BUFSIZE_IN  0x40
uint8_t inbuf[BUFSIZE_IN];
fifo_t infifo;

#define BUFSIZE_OUT 0x40
uint8_t outbuf[BUFSIZE_OUT];
fifo_t outfifo;

void uart_init() {
    uint8_t sreg = SREG;
    uint16_t ubrr = (uint16_t) ((uint32_t) F_CPU/(16*BAUDRATE) - 1);

    UBRRH = (uint8_t) (ubrr>>8);
    UBRRL = (uint8_t) (ubrr);

    // Set UART Pin as output
    DDRD |= _BV(PD1);

    // Interrupts kurz deaktivieren
    cli();

    // UART Receiver und Transmitter anschalten, Receive-Interrupt aktivieren
    // Data mode 8N1, asynchron
    UCSRB = (1 << RXEN) | (1 << TXEN) | (1 << RXCIE);
    //for ATMega8
    //UCSRC = (1 << URSEL) | (1 << UCSZ1) | (1 << UCSZ0);
    UCSRC = (1 << UCSZ1) | (1 << UCSZ0);

    // Flush Receive-Buffer (entfernen evtl. vorhandener ungültiger Werte)
    do
    {
        // UDR auslesen (Wert wird nicht verwendet)
        UDR;
    }
    while (UCSRA & (1 << RXC));

    // Rücksetzen von Receive und Transmit Complete-Flags
    UCSRA = (1 << RXC) | (1 << TXC);

    // Global Interrupt-Flag wieder herstellen
    SREG = sreg;

    // FIFOs für Ein- und Ausgabe initialisieren
    fifo_init (&infifo,   inbuf, BUFSIZE_IN);
    fifo_init (&outfifo, outbuf, BUFSIZE_OUT);
}


// Empfangene Zeichen werden in die Eingabgs-FIFO gespeichert und warten dort
SIGNAL (SIG_UART_RECV) {
    _inline_fifo_put (&infifo, UDR);
}

// Ein Zeichen aus der Ausgabe-FIFO lesen und ausgeben
// Ist das Zeichen fertig ausgegeben, wird ein neuer SIG_UART_DATA-IRQ getriggert
// Ist die FIFO leer, deaktiviert die ISR ihren eigenen IRQ.
SIGNAL (SIG_UART_DATA) {
    if (outfifo.count > 0)
       UDR = _inline_fifo_get (&outfifo);
    else
        UCSRB &= ~(1 << UDRIE);
}

int uart_putc(const uint8_t c) {
    int ret = fifo_put (&outfifo, c);

    UCSRB |= (1 << UDRIE);

    return ret;
}

int uart_putcs(const uint8_t character, FILE *stream) {
	uart_putc(character);
	return 0;
}

int uart_getc_nowait () {
    return fifo_get_nowait (&infifo);
}

uint8_t uart_getc_wait () {
    return fifo_get_wait (&infifo);
}

// Einen 0-terminierten String übertragen.
void uart_puts (const char *s) {
	do {
        uart_putc (*s);
    } while (*s++ && *s != 0);
}
/* unneeded
// Einen 0-terminierten String senden, der im Flash steht.
void uart_puts_P (PGM_P s) {
    while (1) {
        unsigned char c = pgm_read_byte (s);
        s++;
        if ('\0' == c) {
            break;
        }
        uart_putc (c);
    }
}
*/
