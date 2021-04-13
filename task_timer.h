/*
 * task_timer.h
 *
 *  Created on: Apr 10, 2021
 *      Author: Justin Qiao
 */

#ifndef TASK_TIMER_H_
#define TASK_TIMER_H_

#include "main.h"

extern TaskHandle_t Task_Accelerometer_Timer_Handle;

// task_timer is responsible for debouncing and reading inputs from S1 and S2 on MKII
extern volatile bool S1_PRESSED;
extern volatile bool S2_PRESSED;

// task_timer also controls when to generate a enemy square
extern volatile bool GENERATE;

/******************************************************************************
 * Shared timer between all tasks.
 ******************************************************************************/
void Task_Timer(void *pvParameters);


#endif /* TASK_TIMER_H_ */
