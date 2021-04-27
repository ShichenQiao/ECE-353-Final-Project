/*
 * task_timer.h
 *
 *  Created on: Apr 10, 2021
 *      Author: Justin Qiao
 */

#ifndef __TASK_TIMER_H__
#define __TASK_TIMER_H__

#include "main.h"

extern TaskHandle_t Task_Timer_Handle;

// task_timer is responsible for debouncing and reading inputs from S1 and S2 on MKII
extern volatile bool S1_PRESSED;
extern volatile bool S2_PRESSED;

// task_timer also controls when to generate a enemy square
extern volatile bool GENERATE;

/******************************************************************************
 * De-bounce switch S1 on MKII.
 *****************************************************************************/
bool debounce_s1(void);

/******************************************************************************
 * De-bounce switch S2 on MKII.
 *****************************************************************************/
bool debounce_s2(void);

/******************************************************************************
 * Shared timer between all tasks.
 * Initiate an ADC14 conversion every 5mS.
 * Check S1, and S2 states every 5mS.
 * When S2 is pressed during a active game and the ball is not launched, change
 * the color of the tank.
 * Send GENERATE signal to task_enemy every 2 seconds.
 ******************************************************************************/
void Task_Timer(void *pvParameters);


#endif /* __TASK_TIMER_H__ */
