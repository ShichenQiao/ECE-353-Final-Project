/*
 * font_arial.h
 *
 *  Created on: Apr 13, 2021
 *      Author: Justin Qiao
 */

#ifndef __FONT_ARIAL_H__
#define __FONT_ARIAL_H__

#include <stdint.h>

/* Font data for Arial 8pt */
extern const uint8_t arial_8ptBitmaps[];
extern const uint16_t arial_8ptDescriptors[][2];

int get_index(char c);

uint16_t get_width(char c);

uint16_t get_offset(char c);

uint16_t get_height(char c);

#endif
