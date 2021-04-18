/*
 * task_breaker.h
 *
 *  Created on: Apr 10, 2021
 *      Author: Justin Qiao
 */

#ifndef __TASK_BREAKER_H__
#define __TASK_BREAKER_H__

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
extern bool ball_launched;
extern uint8_t ball_x;
extern uint8_t ball_y;
extern int ball_dir;   // 0 for LEFT, 1 for RIGHT, 2 for UP, and 3 for DOWN

// Shared coordinates of the tank
extern uint8_t tank_x;
extern uint8_t tank_y;
extern uint16_t tank_lcd_color;
extern int tank_dir;   // 0 for LEFT, 1 for RIGHT, 2 for UP, and 3 for DOWN

extern bool occupied[LCD_VERTICAL_MAX][LCD_HORIZONTAL_MAX];    // Matrix to record whether a pixel on the LCD is occupied

extern int score;      // Keeps track of the score of a game

/******************************************************************************
 * This function will initialize Queue_Breaker, the LCD and Sem_LCD.
 ******************************************************************************/
void Task_Breaker_Init(void);

/******************************************************************************
 * This is the main task controlling the gaming mode. Commands can be sent to
 * Queue_Breaker to be processed.
 ******************************************************************************/
void Task_Breaker(void *pvParameters);

/******************************************************************************
 * Helper method to reset the ball according to the current position of the
 * tank, the ball will have the same center as the tank, respectively.
 ******************************************************************************/
void ball_reset(void);

/******************************************************************************
 * Helper method to redraw the tank at its current position so that the ball
 * will not cover the tank.
 ******************************************************************************/
void tank_recover();

/******************************************************************************
 * Helper method to get the LCD draw frame of a given image centered at a given
 * coordinates (x, y).
 * (x0, y0) will store the left upper corner of the image.
 * (x0, y0) will store the right lower corner of the image.
 ******************************************************************************/
void get_draw_frame(int x, int y, int image_width_pixels, int image_height_pixels, int *x0, int *x1, int *y0, int *y1);

#endif /* __TASK_BREAKER_H__ */
