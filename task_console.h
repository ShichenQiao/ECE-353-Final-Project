/*
 * task_console.h
 *
 *  Created on: Oct 21, 2020
 *      Author: Joe Krachey
 *
 *  Student Name: Justin Qiao
 */

#ifndef TASK_CONSOLE_H_
#define TASK_CONSOLE_H_

#include <main.h>

extern QueueHandle_t Queue_Console;
extern TaskHandle_t Task_Console_Handle;
extern SemaphoreHandle_t Sem_Console;

/******************************************************************************
 * This function configures the eUSCI_A0 to be a UART that communicates at
 * 115200 8N1.
 * The priority of the eUSCI is 2.
 *
 *  * NOTE: The console window is just for debug purposes for now.
 ******************************************************************************/
void Task_Console_Init(void);


#endif /* TASK_CONSOLE_H_ */
