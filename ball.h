/*
 * ball.h
 *
 *  Created on: Apr 10, 2021
 *      Author: Justin Qiao
 */

#ifndef __BALL_H__
#define __BALL_H__

#include <stdint.h>

/* Bitmap info for ball */
// The out-most rows and columns are NOT leave blank intentionally for precision.
// Have to redraw it in the background color to erase.
extern const uint8_t image_ball[];
extern const uint8_t ballWidthPixels;
extern const uint8_t ballHeightPixels;

#endif /* __BALL_H__ */
