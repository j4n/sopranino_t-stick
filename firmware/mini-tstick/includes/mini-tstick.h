/*
 * mini-tsick.h
 */

#ifndef _MINITSTICK_H
#define _MINITSTICK_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>

#include "adxl345.h"
#include "cy8c20180.h"
#include "io.h"
#include "twimaster.h"
#include "uart.h"
#include "utils.h"

#define UART_BAUD_RATE 57600

//#define SENSOR_COUNT 9 // number of total sensor channels
#define ADC_COUNT 6 // number of adc channels

uint16_t adxl345_values[3]; // three axis
uint8_t adc_values[ADC_COUNT]; // two chips a 8 bits
uint8_t capsense_values[2]; // two chips a 8 bits
const uint8_t adcs[ADC_COUNT] = {0,1,2,3,6,7}; // which ADCs to sample

char buffer[10]; // buffer for sprintf/atoi, max 65535+\0+label+:+tab

uint8_t incoming_char;

void init(void);
void zero_arrays(void);
void sample_adcs(void);
void sample_adxl345(void);
void sample_capsense(void);

void uart_transfer_values_csv(void);
#endif /* _MINI-TSTICK_H */
