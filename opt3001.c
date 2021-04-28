/*
 * opt3001.c
 *
 *  Created on: Mar 14, 2021
 *      Author: younghyunkim, Mark Xia
 *
 */

#include "opt3001.h"
#include "math.h"

static int lux_threshold = 200;

/******************************************************************************
 * Initialize the opt3001 ambient light sensor on the MKII. This function assumes
 * that the I2C interface has already been configured to operate at 100KHz.
 ******************************************************************************/
void opt3001_init(void)
{
    // Initialize OPT3001
    //reset the device using config reg -- data
    i2c_write_16(OPT3001_SLAVE_ADDRESS, CONFIG_REG, 0xC600);
}

/******************************************************************************
 * Turn on BLUE RGB LED if the background color is to be black, else if it will
 * be white, turn on the RED RGB LED.
 ******************************************************************************/
void display_lux(float light)
{
    if(bgc_black)
    {
        ece353_rgb(false, false, true); // turn on BLUE
    }
    else
    {
        ece353_rgb(true, false, false); // turn on RED
    }
}

/******************************************************************************
 * Set the background color by examining the lux.
 ******************************************************************************/
void set_bgc(void)
{
    lux = opt3001_read_lux();  // fetch the lux value.
    if (lux > lux_threshold) {
        bgc_black = false;
    } else {
        bgc_black = true;
    }
    display_lux(lux);
}

/******************************************************************************
 * Returns the current ambient light in lux.
 *******************************************************************************/
float opt3001_read_lux(void)
{
    // Read the Result register of OPT3001 and convert into Lux, then return.
    uint16_t data = i2c_read_16(OPT3001_SLAVE_ADDRESS, RESULT_REG);

    uint16_t E = data >> 12;
    uint16_t R = data - (E << 12);

    return (float)(0.01 * pow(2, E) * R);
}

