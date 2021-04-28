/*
 * task_setting_page.h
 *
 *  Created on: Apr 27, 2021
 *      Author: Justin Qiao
 */

#ifndef __TASK_SETTING_PAGE_H__
#define __TASK_SETTING_PAGE_H__

#include "main.h"

extern TaskHandle_t Task_Setting_Page_Handle;

extern int game_duration_seconds;        // Game duration in seconds.
extern int number_of_colors;        // Number of colors used in game (3 to 11, default 11)

/******************************************************************************
 * Task managing the settings of BREAKER. The game duration, number of colors
 * used in game, and background color of the game board will be configured
 * in this order every time the setting page is entered from the home page.
 ******************************************************************************/
void Task_Setting_Page(void *pvParameters);

/******************************************************************************
 * Set the duration of a game. The possible values are 60s, 90s, ... 300s.
 * Default duration is 120s. Press S1 will change to next level with wrap around.
 * Press S2 will go to next setting.
 ******************************************************************************/
void set_game_duration_seconds();

/******************************************************************************
 * Set the number of colors used in a game. The valid value is between 3 and 11.
 * the default value is 11. Press S1 will change to next level with wrap around.
 * Press S2 will go to next setting.
 ******************************************************************************/
void set_number_of_colors();

#endif /* __TASK_SETTING_PAGE_H__ */
