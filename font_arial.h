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

/******************************************************************************
 * Get the row index of a char c in arial_8ptDescriptors.
 ******************************************************************************/
int get_index(char c);

/******************************************************************************
 * Get the width of a char c in Airal_8pt font.
 ******************************************************************************/
uint16_t get_width(char c);

/******************************************************************************
 * Get the offset of a char c in arial_8ptBitmaps[].
 ******************************************************************************/
uint16_t get_offset(char c);

/******************************************************************************
 * Get the height of a char c in Airal_8pt font.
 ******************************************************************************/
uint16_t get_height(char c);

#endif /* __FONT_ARIAL_H__ */
