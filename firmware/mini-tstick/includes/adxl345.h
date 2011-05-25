/*
 * adxl345.h
 *
 *  Created on: Jul 19, 2010
 *      Author: szehe
 */

#ifndef _ADXL345_H
#define _ADXL345_H

#include <stdio.h>
#include <avr/io.h>
#include "io.h"
#include "utils.h"
#include "i2cmaster.h"

//#define ADXL345_ADDR 0xA6 // SDO tied to GND
#define ADXL345_ADDR 0x3A // SDO tied to VCC

void adxl345_init(void);
uint8_t adxl345_read(uint8_t register_name);
void adxl345_write(uint8_t register_name, uint8_t value);
void adxl345_read_values(uint16_t* adxl345_values);

unsigned char acc_x_highbyte;
unsigned char acc_x_lowbyte;
unsigned char acc_y_highbyte;
unsigned char acc_y_lowbyte;
unsigned char acc_z_highbyte;
unsigned char acc_z_lowbyte;

#endif
