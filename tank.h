/*
 * tank.h
 *
 *  Created on: Apr 13, 2021
 *      Author: Justin Qiao
 */

#ifndef __TANK_H__
#define __TANK_H__

#include <stdint.h>

/* Bitmap info for tank */
extern const uint8_t tank_upBitmaps[];
extern const uint8_t tank_downBitmaps[];
extern const uint8_t tank_rightBitmaps[];
extern const uint8_t tank_leftBitmaps[];

// No matter in which direction, the width and height are fixed
extern const uint8_t tankWidthPixels;
extern const uint8_t tankHeightPixels;

#endif /* __TANK_H__ */
