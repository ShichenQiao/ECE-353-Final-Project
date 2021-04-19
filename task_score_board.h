/*
 * task_score_board.h
 *
 *  Created on: Apr 14, 2021
 *      Author: Justin Qiao
 */

#ifndef __TASK_SCORE_BOARD_H__
#define __TASK_SCORE_BOARD_H__

#include <main.h>

extern TaskHandle_t Task_Score_Board_Handle;

/******************************************************************************
 * This function resets the score board. It will be called in reset_game().
 ******************************************************************************/
void score_board_reset();

/******************************************************************************
 * This task manages the score board in Gaming mode. This function will count
 * down the time left in an ongoing game, and terminates the game when time is
 * up. The score section of the score board will be managed by update_score(),
 * which updates the score shown on LCD only when a color-matched collision
 * between the ball and a square occurs.
 ******************************************************************************/
void Task_Score_Board(void *pvParameters);

/******************************************************************************
 * Helper method to parse an int num into three digits as chars. The hundreds
 * place will be stored in *a, the tens place will be stored in *b, and the
 * ones place will be stored in *c.
 * If a place does not exist in num, set its corresponding char to '0'.
 * If num is greater than 999, set all of a, b, and c to '9'.
 ******************************************************************************/
void int_to_three_chars(int num, char *a, char *b, char *c);

/******************************************************************************
 * This function manages the score section on the score board. It will be called
 * from check_squares() as defined in task_enemy when a color-matched collision
 * between the ball and a square occurs. This function will update the score
 * shown on LCD accordingly when called.
 ******************************************************************************/
void update_score();

#endif /* __TASK_SCORE_BOARD_H__ */
