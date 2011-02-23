// Basic IO Functionality Support Code (LEDs etc.)
// Copyright 2010 Jan Anlauff <jan at 23t.de>

#include "includes/io.h"

void led_init(void) {
	LED_PORT_REGISTER |= _BV(LED_PIN);
}

void led_on(void) {
	LED_PORT |= _BV(LED_PIN);
}

void led_off(void) {
	LED_PORT &= ~_BV(LED_PIN);
}

void led_toggle() {
	if (bit_is_set(LED_PORT, LED_PIN)) {
		led_off();
	} else {
		led_on();
	}
}

void adc_init() {
	/* internal pull-ups interfere with the ADC so disable them
	 * p260: Note that ADC pins ADC7 and ADC6 do not have digital input
	 * buffers, and therefore do not require Digital Input Disable bits.
	 */
	ADC_PORT = 0x00;
	ADC_PORT_REGISTER = 0x00;

	/* unless otherwise configured, arduinos use the internal Vcc
	 * reference. MUX 0x0f samples the ground (0.0V). */
	ADMUX = _BV(REFS0) | 0x0f;

	/*
	 * Enable the ADC system, use 128 as the clock divider on a 16MHz
	 * arduino (ADC needs a 50 - 200kHz clock) and start a sample. the
	 * AVR needs to do some set-up the first time the ADC is used; this
	 * first, discarded, sample primes the system for later use.
	 */
	ADCSRA |= _BV(ADEN) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0) | _BV(ADSC);
	// wait for the ADC to return a sample
	loop_until_bit_is_clear(ADCSRA, ADSC);
}

uint16_t adc_read(uint8_t pin) {
	unsigned char low_adc_byte, high_adc_byte, input_channel;

	input_channel = (ADMUX & 0xf0) | (pin & 0x0f);  // p257
	ADMUX = input_channel; // select the input channel
	ADCSRA |= _BV(ADSC); // start conversion
	loop_until_bit_is_clear(ADCSRA, ADSC); // wait until complete

	// must read the low ADC byte before the high ADC byte
	low_adc_byte = ADCL;
	high_adc_byte = ADCH;

	return ((uint16_t)high_adc_byte << 8) | low_adc_byte;
}
