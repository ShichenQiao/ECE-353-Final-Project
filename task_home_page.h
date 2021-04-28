/*
 * task_home_page.h
 *
 *  Created on: Apr 27, 2021
 *      Author: Justin Qiao, Mark Xia
 */

#ifndef __TASK_HOME_PAGE_H__
#define __TASK_HOME_PAGE_H__

#include "main.h"

extern TaskHandle_t Task_Home_Page_Handle;

/******************************************************************************
 * Home page of the game BREAKER. Plays theme song with LED flashing.
 * Press S1 to go to Gaming mode.
 * Press S2 to go to Setting mode.
 ******************************************************************************/
void Task_Home_Page(void *pvParameters);


#endif /* __TASK_HOME_PAGE_H__ */
