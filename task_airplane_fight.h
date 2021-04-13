/*
 * task_airplane_fight.h
 *
 *  Created on: Apr 10, 2021
 *      Author: Justin Qiao
 */

#ifndef TASK_AIRPLANE_FIGHT_H_
#define TASK_AIRPLANE_FIGHT_H_

#include <main.h>

typedef enum {
    BAR_CMD_LEFT,
    BAR_CMD_RIGHT,
    BAR_CMD_UP,
    BAR_CMD_DOWN,
    BAR_CMD_CENTER
} BAR_CMD_t;

extern TaskHandle_t Task_Airplane_Fight_Handle;
extern QueueHandle_t Queue_Airplane_Fight;

// Shared information about the ball
extern bool LAUNCHED;
extern uint8_t ball_x;
extern uint8_t ball_y;
extern uint16_t ball_lcd_color;

/******************************************************************************
 * This function will initialize Queue_Airplane_Fight and initialize the LCD
 ******************************************************************************/
void Task_Airplane_Fight_Init(void);

/******************************************************************************
 * This is the main task controlling the gaming mode. Commands can be sent to
 * Queue_Airplane_Fight to be processed.
 ******************************************************************************/
void Task_Airplane_Fight(void *pvParameters);

/******************************************************************************
 * Helper method to reset the ball according to the current position of the
 * airplane, the ball will be placed 1 pixel above the airplane, centered at
 * the same x axis, respectively.
 ******************************************************************************/
void ball_reset(void);

#endif /* TASK_AIRPLANE_FIGHT_H_ */
