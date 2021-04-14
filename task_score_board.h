/*
 * task_score_board.h
 *
 *  Created on: Apr 14, 2021
 *      Author: Justin Qiao
 */

#ifndef TASK_SCORE_BOARD_H_
#define TASK_SCORE_BOARD_H_

#include <main.h>

extern TaskHandle_t Task_Score_Board_Handle;

/******************************************************************************
 * This task manages the score board in Gaming mode
 ******************************************************************************/
void Task_Score_Board(void *pvParameters);

/******************************************************************************
 * Helper method to print char c in arial on the LCD screen, centered at (x, y)
 ******************************************************************************/
void lcd_print_char(uint16_t x, uint16_t y, char c);


#endif /* TASK_SCORE_BOARD_H_ */
