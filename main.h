/*
 * main.h
 *
 *  Created on: Apr 10, 2021
 *      Author: Justin Qiao
 */

#ifndef __MAIN_H__
#define __MAIN_H__

#include "msp.h"
#include "msp432p401r.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

/* RTOS header files */
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>

/* ECE353 Includes */
#include <ece353.h>
#include "opt3001.h"
#include "i2c.h"
#include <lcd.h>

/* Image data */
#include <ball.h>
#include <buzzer.h>
#include <font_arial.h>
#include <tank.h>
#include <logo.h>

/* FreeRTOS Tasks */
#include <task_accelerometer.h>
#include <task_breaker.h>
#include <task_console.h>
#include <task_enemy.h>
#include <task_score_board.h>
#include <task_timer.h>
#include <task_home_page.h>
#include <task_setting_page.h>


#endif /* __MAIN_H__ */
