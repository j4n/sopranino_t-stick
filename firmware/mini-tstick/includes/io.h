// Basic IO Functionality Support Code (LEDs etc.)
// Copyright 2010 Jan Anlauff <jan at 23t.de>

#ifndef IO_H_
#define IO_H_

#include <avr/pgmspace.h>

// mini-tstick
#define LED_PIN PD7
#define LED_PORT PORTD
#define LED_PORT_REGISTER DDRD

#define ADC_PORT PORTC
#define ADC_PORT_REGISTER DDRC
#define ADC_PORT_MASK 0b00110000 // which ports are used as adcs, exclude i2c pins 4+5

void led_init (void);
void led_on(void);
void led_off(void);
void led_toggle(void);

void adc_init(void);
uint16_t adc_read(uint8_t);

void digital_out_init(void);
void digital_write(uint8_t, uint8_t);

#endif /* IO_H_ */
