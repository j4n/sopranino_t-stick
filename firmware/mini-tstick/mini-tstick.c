/*
 * Mini T-Stick Controller Firmware
 *
 *  Created by Jan Anlauff <janlauff at cim dot mcgill dot ca> (C) 2011
 *  ADXL345 functionality based on code from Sebastian Zehe
 *  I2C master code by Peter Fleury 
 */

#include "includes/mini-tstick.h"

int main(void) {
	init();
	while (1) {
		incoming_char = uart_getc_wait();
		sample_adcs();
		led_off();
		sample_adxl345();
		led_on();
		_delay_ms(1);
		uart_transfer_values_csv();
	}
}

void init(void) {
	led_init();
	led_off();
	sei(); // Turn on interrupts
	uart_init();
	FILE uart_str = FDEV_SETUP_STREAM(uart_putcs, NULL, _FDEV_SETUP_RW);
	stdout = &uart_str;
	adc_init();
	twi_init();
	adxl345_init();
	led_on();
}

void sample_adcs() {
	for(uint8_t adc_id = 0; adc_id < ADC_COUNT; adc_id++) {
		values[adc_ids[adc_id]] = adc_read(adc_id);
	}
}

void sample_adxl345(void) {
	adxl345_read_values(adxl345_values);
	for(uint8_t axis=0; axis < 3; axis++) {
		values[adxl345_ids[axis]] = adxl345_values[axis];
	}
}

void uart_transfer_values_csv(void) {
	for (uint8_t sensor_id = 0; sensor_id < SENSOR_COUNT; sensor_id++) {
		itoa(values[sensor_id], buffer, 10);
		uart_puts(buffer);
		uart_puts(",");
	}
	uart_puts("\r\n");
	uart_flush();
}

