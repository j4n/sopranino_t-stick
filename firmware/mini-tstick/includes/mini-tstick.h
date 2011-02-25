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

#define SENSOR_COUNT 9 // number of total sensor channels
#define ADC_COUNT 6 // number of adc channels

uint16_t values[SENSOR_COUNT]; // stores all sensor values
uint16_t adxl345_values[3];
char buffer[10]; // buffer for sprintf/atoi, max 65535+\0+label+:+tab

// labels of all sensor channels
const char sensor_labels[SENSOR_COUNT] = "ABCDEFGHI";
// which of those channels are ADCs (some are read via SPI)
const uint8_t adc_ids[ADC_COUNT] = {0,1,2,3,4,5};
// which ADCs to sample
const uint8_t adcs[ADC_COUNT] = {0,1,2,3,6,7};
// which of those are adxl345 channels
const uint8_t adxl345_ids[3] = {6,7,8};

uint8_t incoming_char;

void init(void);
void sample_adcs(void);
void sample_adxl345(void);
void sample_capsens(void);

void uart_transfer_values_csv(void);
#endif /* _MINI-TSTICK_H */
