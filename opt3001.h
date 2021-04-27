/*
 * opt3001.h
 *
 *  Created on: Mar 14, 2021
 *      Author: Younghyun Kim, Mark Xia
 */

#ifndef OPT3001_H
#define OPT3001_H

#include "msp.h"
#include "i2c.h"
#include "ece353.h"
#include <stdint.h>
#include <stdbool.h>

// 
#define OPT3001_SLAVE_ADDRESS 0x44/* ADD CODE */

#define OPT_INTERRUPT_PIN 11
#define RESULT_REG 0x00/* ADD CODE */
#define CONFIG_REG 0x01/* ADD CODE */
#define LOWLIMIT_REG 0x02
#define HIGHLIMIT_REG 0x03
#define MANUFACTUREID_REG 0x7E
#define DEVICEID_REG 0x7F

/******************************************************************************
 * Initialize the opt3001 ambient light sensor on the MKII.  This function assumes
 * that the I2C interface has already been configured to operate at 100KHz.
 ******************************************************************************/
void opt3001_init(void);

/******************************************************************************
 * Returns the current ambient light in lux
 ******************************************************************************/
float opt3001_read_lux(void);

void display_lux(float light);

void set_bgc(void);

#endif /* OPT3001_H */
