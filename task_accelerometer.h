/*
 * task_accelerometer.h
 *
 *  Created on: Apr 10, 2021
 *      Author: Justin Qiao
 */

#ifndef __TASK_ACCELEROMETER_H__
#define __TASK_ACCELEROMETER_H__

#include "main.h"

#define VOLT_1P40  1738      // 1.40 /(3.3/4096)
#define VOLT_1P80  2234      // 1.70 /(3.3/4096)

extern TaskHandle_t Task_Accelerometer_Handle;

/******************************************************************************
* Configure the IO pins for BOTH the X and Y directions of the analog
* Accelerometer. The X direction should be configured to place the results in
* MEM[0]. The Y direction should be configured to place the results in MEM[1].
*
* After BOTH analog signals have finished being converted, a SINGLE interrupt
* will be generated.
******************************************************************************/
void Task_Accelerometer_Init(void);

/******************************************************************************
* Examines the ADC data from the Accelerometer on the MKII.
******************************************************************************/
void Task_Accelerometer_Bottom_Half(void *pvParameters);

#endif /* __TASK_ACCELEROMETER_H__ */
