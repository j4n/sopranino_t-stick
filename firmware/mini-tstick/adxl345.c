/*
 * adxl345.c
 *
 * Based on code by Sebastian Zehe
 */

#include "includes/adxl345.h"
void adxl345_init(void) {
	//set adxl to measurement mode
	twi_start(0xA6 + I2C_WRITE);
	twi_write(0x2D);
	//set measurement bit (bit 3)
	twi_write(0x08);
	twi_stop();
	long_delay(10);
}

uint8_t adxl345_read(uint8_t register_name) {
	uint8_t ret;
	twi_start(0xA6 + I2C_WRITE);
	twi_write(register_name);
	twi_rep_start(0xA6 + I2C_READ);
	ret = twi_readNak();
	twi_stop();
	return ret;
}

void adxl345_write(uint8_t register_name, uint8_t value) {
	twi_start(0xA6 + I2C_WRITE);
	twi_write(register_name);
	twi_write(value);
	twi_stop();
}

void adxl345_read_values(uint16_t* adxl345_values) {
	//read adxl345 x-axis lowbyte
	twi_start(0xA6 + I2C_WRITE);
	twi_write(0x32);
	twi_rep_start(0xA6 + I2C_READ);
	acc_x_lowbyte = twi_readNak();

	//read adxl345 x-axis highbyte
	twi_rep_start(0xA6 + I2C_WRITE);
	twi_write(0x33);
	twi_rep_start(0xA6 + I2C_READ);
	acc_x_highbyte = twi_readNak();

	//read adxl345 y-axis lowbyte
	twi_rep_start(0xA6 + I2C_WRITE);
	twi_write(0x34);
	twi_rep_start(0xA6 + I2C_READ);
	acc_y_lowbyte = twi_readNak();

	//read adxl345 y-axis highbyte
	twi_rep_start(0xA6 + I2C_WRITE);
	twi_write(0x35);
	twi_rep_start(0xA6 + I2C_READ);
	acc_y_highbyte = twi_readNak();

	//read adxl345 z-axis lowbyte
	twi_rep_start(0xA6 + I2C_WRITE);
	twi_write(0x36);
	twi_rep_start(0xA6 + I2C_READ);
	acc_z_lowbyte = twi_readNak();

	//read adxl345 z-axis highbyte
	twi_rep_start(0xA6 + I2C_WRITE);
	twi_write(0x37);
	twi_rep_start(0xA6 + I2C_READ);
	acc_z_highbyte = twi_readNak();

	twi_stop();

	//bytes to integers, fill array
	adxl345_values[0] = make_u16(acc_x_highbyte, acc_x_lowbyte);
	adxl345_values[1] = make_u16(acc_y_highbyte, acc_y_lowbyte);
	adxl345_values[2] = make_u16(acc_z_highbyte, acc_z_lowbyte);
}

