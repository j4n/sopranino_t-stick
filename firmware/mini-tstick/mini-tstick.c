/*
 * Mini T-Stick Controller Firmware
 *
 *  Created by Jan Anlauff <janlauff at cim dot mcgill dot ca> (C) 2011
 *  ADXL345 functionality based on code from Sebastian Zehe
 *  I2C master library by Peter Fleury 
 *
 *
 *  TODO:
 *  	- finish capsense read
 */

#include "includes/mini-tstick.h"

int main(void) {
	init();
	while (1) {
		//incoming_char = uart_getc_wait();
		sample_adcs();
		//led_off();
//		sample_adxl345();
		//led_on();
		sample_capsense();
		_delay_ms(10);
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
	zero_arrays();
	adc_init();
	i2c_init();
//	adxl345_init();
	//cy8c20180_config(); // not needed, stored in NVM
	//led_on();
}

void sample_adcs() {
	for(uint8_t adc = 0; adc< ADC_COUNT; adc++) {
		adc_values[adc] = adc_read(adcs[adc]);
	}
}

void sample_adxl345(void) {
	adxl345_read_values(adxl345_values);
}

void sample_capsense() {
		capsense_values[0] = cy8c20180_read(0x00);
		capsense_values[1] = cy8c20180_read(0x01);
}

void uart_transfer_values_csv(void) {
	// print adc
	for(uint8_t adc = 0; adc < ADC_COUNT; adc++) {
		itoa(adc_values[adc], buffer, 10);
		uart_puts(buffer);
		uart_puts(",");
	}

	// print adxl345
	for(uint8_t axis = 0; axis <= 2; axis++) {
		itoa(adxl345_values[axis], buffer, 10);
		uart_puts(buffer);
		uart_puts(",");
	}

	// print capsense
	for(uint8_t capsense_chip = 0; capsense_chip <= 1; capsense_chip++) {
		itoa(capsense_values[capsense_chip], buffer, 10);
		uart_puts(buffer);
		uart_puts(",");
	}

	uart_puts("\r\n");
	uart_flush();
}

void zero_arrays(void) {
	for(uint8_t adc = 0; adc < ADC_COUNT; adc++) {
		adc_values[adc] = 17;
	}

	for(uint8_t axis = 0; axis <= 2; axis++) {
		adxl345_values[axis] = 23;
	}

	for(uint8_t capsense_chip = 0; capsense_chip <= 1; capsense_chip++) {
		capsense_values[capsense_chip] = 42;
	}
}

