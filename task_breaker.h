/*
 * task_breaker.h
 *
 *  Created on: Apr 10, 2021
 *      Author: Justin Qiao
 */

#ifndef __TASK_BREAKER_H__
#define __TASK_BREAKER_H__

#include <main.h>

// All possible commands to Queue_Breaker
typedef enum {
    TANK_CMD_LEFT,
    TANK_CMD_RIGHT,
    TANK_CMD_UP,
    TANK_CMD_DOWN,
    TANK_CMD_CENTER,
    TANK_CMD_LAUNCH
} TANK_CMD_t;

extern TaskHandle_t Task_Breaker_Handle;
extern QueueHandle_t Queue_Breaker;
extern SemaphoreHandle_t Sem_LCD;

// Shared information about the ball
extern uint8_t ball_x;
extern uint8_t ball_y;
extern bool ball_launched;      // Flag variable, true if the ball has been launched
extern int ball_dir;   // 0 for LEFT, 1 for RIGHT, 2 for UP, and 3 for DOWN

// Shared coordinates of the tank
extern uint8_t tank_x;
extern uint8_t tank_y;
extern uint16_t tank_lcd_color;
extern int tank_dir;   // 0 for LEFT, 1 for RIGHT, 2 for UP, and 3 for DOWN

// Lux reading from the light sensor
extern float lux;

// Matrix to record whether a pixel on the LCD is occupied
extern bool occupied[LCD_VERTICAL_MAX][LCD_HORIZONTAL_MAX];

extern int score;      // Keeps track of the score of a game
extern int time_left;      // The time left in an ongoing game
extern bool game_on_going;        // True if a game is ongoing, false otherwise

/******************************************************************************
 * This function will initialize Queue_Breaker, the LCD and Sem_LCD.
 ******************************************************************************/
void Task_Breaker_Init(void);

/******************************************************************************
 * This function resets a Breaker game. All game parameters will be set to
 * default, and the LCD will be redraw as its initial condition.
 ******************************************************************************/
void reset_game();

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
 * Helper method to print a pre-game message with instructions.
 ******************************************************************************/
void print_pre_game_message();

/******************************************************************************
 * Helper method to print a message saying game over and show the score earned.
 * Instruction for how to proceed will also be printed after a 5 second pause
 * with count down on LCD.
 ******************************************************************************/
void print_end_game_message();

#endif /* __TASK_BREAKER_H__ */
