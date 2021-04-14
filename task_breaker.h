/*
 * task_breaker.h
 *
 *  Created on: Apr 10, 2021
 *      Author: Justin Qiao
 */

#ifndef TASK_BREAKER_H_
#define TASK_BREAKER_H_

#include <main.h>

typedef enum {
    BAR_CMD_LEFT,
    BAR_CMD_RIGHT,
    BAR_CMD_UP,
    BAR_CMD_DOWN,
    BAR_CMD_CENTER
} BAR_CMD_t;

extern TaskHandle_t Task_Breaker_Handle;
extern QueueHandle_t Queue_Breaker;
extern SemaphoreHandle_t Sem_LCD;

// Shared information about the ball
extern bool LAUNCHED;
extern uint8_t ball_x;
extern uint8_t ball_y;
extern uint16_t ball_lcd_color;

// Shared coordinates of the tank
extern uint8_t tank_x;
extern uint8_t tank_y;
extern int tank_dir;   // 0 for LEFT, 1 for RIGHT, 2 for UP, and 3 for DOWN

/******************************************************************************
 * This function will initialize Queue_Breaker, initialize the LCD and initialize
 * Sem_LCD.
 ******************************************************************************/
void Task_Breaker_Init(void);

/******************************************************************************
 * This is the main task controlling the gaming mode. Commands can be sent to
 * Queue_Breaker to be processed.
 ******************************************************************************/
void Task_Breaker(void *pvParameters);

/******************************************************************************
 * Helper method to reset the ball according to the current position of the
 * tank, the ball will be placed 1 pixel above the tank, centered at
 * the same x axis, respectively.
 ******************************************************************************/
void ball_reset(void);

#endif /* TASK_BREAKER_H_ */
