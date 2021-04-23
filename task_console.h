/*
 * task_console.h
 *
 *  Created on: Apr 22, 2021
 *      Author: Justin Qiao
 */

#ifndef __TASK_CONSOLE_H__
#define __TASK_CONSOLE_H__

#include <main.h>

extern TaskHandle_t Task_Console_Handle;
extern SemaphoreHandle_t Sem_Console;

extern volatile uint16_t RX_INDEX;

/******************************************************************************
 * This function configures the eUSCI_A0 to be a UART that communicates at
 * 115200 8N1.
 * The priority of the eUSCI is 2.
 ******************************************************************************/
static void console_hw_init(void);

/******************************************************************************
 * This function configures the eUSCI_A0 to be a UART that communicates at
 * 115200 8N1.
 * The priority of the eUSCI is 2.
 *
 *  * NOTE: The console window is just for debug purposes for now.
 ******************************************************************************/
void Task_Console_Init(void);

/******************************************************************************
* Task used to send command from the console window to task_breaker.
******************************************************************************/
void Task_Console_Bottom_Half(void *pvParameters);

#endif /* __TASK_CONSOLE_H__ */
