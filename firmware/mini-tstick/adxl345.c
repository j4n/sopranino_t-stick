/*
 * adxl345.c
 *
 * Based on code by Sebastian Zehe
 */

#include "includes/adxl345.h"
void adxl345_init(void) {
	//set adxl to constant measurement mode
	i2c_start(ADXL345_ADDR + I2C_WRITE);
	i2c_write(0x2D);
	//set measurement bit (bit 3)
	i2c_write( (1<<3) );
	i2c_stop();
	long_delay(10);
}

uint8_t adxl345_read(uint8_t register_name) {
	uint8_t ret;
	i2c_start(ADXL345_ADDR + I2C_WRITE);
	i2c_write(register_name);
	i2c_rep_start(ADXL345_ADDR + I2C_READ);
	ret = i2c_readNak();
	i2c_stop();
	return ret;
}

void adxl345_write(uint8_t register_name, uint8_t value) {
	i2c_start(ADXL345_ADDR + I2C_WRITE);
	i2c_write(register_name);
	i2c_write(value);
	i2c_stop();
}

void adxl345_read_values(uint16_t* adxl345_values) {
	//read adxl345 x-axis lowbyte
	i2c_start(ADXL345_ADDR + I2C_WRITE);
	i2c_write(0x32);
	i2c_rep_start(ADXL345_ADDR + I2C_READ);
	acc_x_lowbyte = i2c_readNak();

	//read adxl345 x-axis highbyte
	i2c_rep_start(ADXL345_ADDR + I2C_WRITE);
	i2c_write(0x33);
	i2c_rep_start(ADXL345_ADDR + I2C_READ);
	acc_x_highbyte = i2c_readNak();

	//read adxl345 y-axis lowbyte
	i2c_rep_start(ADXL345_ADDR + I2C_WRITE);
	i2c_write(0x34);
	i2c_rep_start(ADXL345_ADDR + I2C_READ);
	acc_y_lowbyte = i2c_readNak();

	//read adxl345 y-axis highbyte
	i2c_rep_start(ADXL345_ADDR + I2C_WRITE);
	i2c_write(0x35);
	i2c_rep_start(ADXL345_ADDR + I2C_READ);
	acc_y_highbyte = i2c_readNak();

	//read adxl345 z-axis lowbyte
	i2c_rep_start(ADXL345_ADDR + I2C_WRITE);
	i2c_write(0x36);
	i2c_rep_start(ADXL345_ADDR + I2C_READ);
	acc_z_lowbyte = i2c_readNak();

	//read adxl345 z-axis highbyte
	i2c_rep_start(ADXL345_ADDR + I2C_WRITE);
	i2c_write(0x37);
	i2c_rep_start(ADXL345_ADDR + I2C_READ);
	acc_z_highbyte = i2c_readNak();

	i2c_stop();

	//bytes to integers, fill array
	adxl345_values[0] = make_u16(acc_x_highbyte, acc_x_lowbyte);
	adxl345_values[1] = make_u16(acc_y_highbyte, acc_y_lowbyte);
	adxl345_values[2] = make_u16(acc_z_highbyte, acc_z_lowbyte);
}

