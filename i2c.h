/*
 * i2c.h
 *
 *  Created on: Sep 25, 2020
 *      Author: Joe Krachey
 */

#ifndef __I2C_H__
#define __I2C_H__

#include "msp.h"
#include <stdint.h>
#include "main.h"

extern float lux;
extern bool bgc_black;

/**********************************************************************************************
 * Initialize all ports and pins needed for i2c bus
 **********************************************************************************************/
void i2c_init(void);

/**********************************************************************************************
 * Read 16 bits data from i2c bus
 **********************************************************************************************/
uint16_t i2c_read_16(uint8_t slave_address, uint8_t dev_address);

/**********************************************************************************************
 * Write 16 bits data into i2c bus
 **********************************************************************************************/
void i2c_write_16(uint8_t slave_address, uint8_t dev_address, uint16_t data);

#endif /* __I2C_H__ */
