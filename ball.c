/*
 * ball.c
 *
 *  Created on: Apr 10, 2021
 *      Author: Justin Qiao
 */

/* 
 *  Image data for the ball
 */
#include <ball.h>

// The out most rows and columns are NOT blank intentionally for precision.
// Have to redraw it in background color to erase.
const uint8_t image_ball[] =
{
    0x1E, 0x00, //       ####
    0x7F, 0x80, //     ########
    0x7F, 0x80, //     ########
    0xFF, 0xC0, //    ##########
    0xFF, 0xC0, //    ##########
    0xFF, 0xC0, //    ##########
    0x7F, 0x80, //     ########
    0x7F, 0x80, //     ########
    0x1E, 0x00, //       ####
};

/* Bitmap sizes for ball */
const uint8_t ballWidthPixels = 10;
const uint8_t ballHeightPixels = 9;

