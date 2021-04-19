/*
 * task_enemy.h
 *
 *  Created on: Apr 10, 2021
 *      Author: Justin Qiao
 */

#ifndef __TASK_ENEMY_H__
#define __TASK_ENEMY_H__

#include <main.h>
#include <time.h>

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

// Shared information about the tank
extern Color_t tank_color;

typedef struct{
    Color_t color;
    bool occupied;
} Enemy_t;

/******************************************************************************
 * This function resets the enemies. It will be called in reset_game().
 ******************************************************************************/
void enemy_reset();

/******************************************************************************
 * This task manages the random generations and breaks of enemy squares.
 * Each enemy square will have a Width = Height = 12.
 ******************************************************************************/
void Task_Enemy(void *pvParameters);

/******************************************************************************
 * Helper method to convert a Color_t to uint16_t (LCD colors).
 ******************************************************************************/
uint16_t get_lcd_color(Color_t color);

/******************************************************************************
 * Helper method to solve collision conditions between the ball and the squares.
 * Two corners of the ball, as specified in Task_Enemy will be passed to this
 * method as (xk_1, yk_1) followed by (xk_2, yk_2).
 * This method will break any color-matched squares in a collision.
 * This method will randomly re-color any color-mismatched squares in a collision.
 * This method will also reset the ball if needed.
 * This method will appear to be dealing with the two given squares simultaneously,
 * and it does not matter if the given coordinate pairs belongs to the same square.
 ******************************************************************************/
void check_squares(int xk_1, int yk_1, int xk_2, int yk_2);

#endif /* __TASK_ENEMY_H__ */
