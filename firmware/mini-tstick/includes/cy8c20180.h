// Cypress CY8C20180 I2C Library
// supports two chips, the second one needs a digital output on the uc 
// connected to the XRES pin
// based on Joseph Mallochs example code
// (c) 2011 Jan Anlauf <janlauff <at> cim <dot> mcgill <dot> ca>

#ifndef CY8C20180_H_
#define CY8C20180_H_

#include <avr/pgmspace.h>
#include "utils.h"
#include "twimaster.h"

// Capsense Chip XRES Reset Pin
#define XRES_PIN PD5
#define XRES_PORT PORTD
#define XRES_PORT_REGISTER DDRD

// I2C adresses
#define I2C_ADDR0 0x00
#define I2C_ADDR1 0x01

// some CY8C201xx registers
#define INPUT_PORT0 0x00
#define INPUT_PORT1 0x01
#define CS_ENABLE0 0x06
#define CS_ENABLE1 0x07
#define I2C_DEV_LOCK 0x79
#define I2C_ADDR_DM 0x7C
#define COMMAND_REG 0xA0

void cy8c20180_config (void);
uint8_t* cy8c20180_read (void);

#endif
