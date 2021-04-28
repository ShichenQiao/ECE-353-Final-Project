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

extern int number_of_colors;

/******************************************************************************
 *
 ******************************************************************************/
void Task_Setting_Page(void *pvParameters);

/******************************************************************************
 *
 ******************************************************************************/
void set_number_of_colors();


#endif /* __TASK_SETTING_PAGE_H__ */
