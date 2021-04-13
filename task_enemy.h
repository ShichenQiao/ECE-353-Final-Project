/*
 * task_enemy.h
 *
 *  Created on: Apr 10, 2021
 *      Author: Justin Qiao
 */

#ifndef TASK_ENEMY_H_
#define TASK_ENEMY_H_

#include <main.h>

extern TaskHandle_t Task_Enemy_Handle;

typedef enum {
    RED,
    GREEN,
    GREEN2,
    BLUE,
    BLUE2,
    YELLOW,
    ORANGE,
    CYAN,
    MAGENTA,
    GRAY,
    BROWN
} Color_t;

// Shared information about the ball
extern Color_t ball_color;

typedef struct{
    Color_t color;
    bool occupied;
} Enemy_t;

/******************************************************************************
 * This task manages the random generations and breaks of enemy squares.
 ******************************************************************************/
void Task_Enemy(void *pvParameters);

/******************************************************************************
 * Helper method to convert a Color_t to uint16_t (LCD colors).
 ******************************************************************************/
uint16_t get_lcd_color(Color_t color);

#endif /* TASK_ENEMY_H_ */
