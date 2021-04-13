/*
 * task_airplane_fight.c
 *
 *  Created on: Apr 10, 2021
 *      Author: Justin Qiao
 */

#include <main.h>

#define QUEUE_LEN  2

TaskHandle_t Task_Airplane_Fight_Handle;
QueueHandle_t Queue_Airplane_Fight;

// Shared information about the ball
uint8_t ball_x;
uint8_t ball_y;
Color_t ball_color;
uint16_t ball_lcd_color;
bool LAUNCHED = false;      // Flag variable, true if the ball has been launched

// Coordinates of the airplane, restricted to this file
uint8_t airplane_x;
uint8_t airplane_y;

/******************************************************************************
 * This function will initialize Queue_Airplane_Fight and initialize the LCD
 ******************************************************************************/
void Task_Airplane_Fight_Init(void)
{
    Queue_Airplane_Fight = xQueueCreate(QUEUE_LEN, sizeof(BAR_CMD_t));

    Crystalfontz128x128_Init();
}

/******************************************************************************
 * This is the main task controlling the gaming mode. Commands can be sent to
 * Queue_Airplane_Fight to be processed.
 ******************************************************************************/
void Task_Airplane_Fight(void *pvParameters)
{
    // Initialize position of the airplane
    airplane_x = 67;
    airplane_y = 115;

    // Flag variable, the airplane shown on LCD will only updates if this variable is true
    bool airplane_update = true;

    // Initialize position and color of the ball
    ball_x = 67;
    ball_y = 95;
    ball_color = GREEN;
    ball_lcd_color = get_lcd_color(ball_color);

    // Helper variables to recored previous position of the ball right befor LCD updates
    uint8_t ball_prev_x;
    uint8_t ball_prev_y;

    // Draw the initial image of the ball
    lcd_draw_image(
            ball_x,
            ball_y,
            ballWidthPixels,
            9,
            image_ball,
            ball_lcd_color,
            LCD_COLOR_BLACK
    );

    // Draw the initial image of the airplane
    lcd_draw_image(
            airplane_x,
            airplane_y,
            airplaneWidthPixels,
            airplaneHeightPixels,
            airplaneBitmaps,
            LCD_COLOR_BLUE,
            LCD_COLOR_BLACK
    );

    // Draw boarders on LCD
    lcd_draw_rectangle(4, 67, 5, 128, LCD_COLOR_RED);       // Left boarder
    lcd_draw_rectangle(127, 67, 5, 128, LCD_COLOR_RED);       // Right boarder

    // What until S1 is pressed to start a game
    while(!S1_PRESSED){};
    S1_PRESSED = false;

    // Enter the Gaming Mode
    while(1)
    {
        BAR_CMD_t message;     // Message received from Queue_Airplane_Fight

        airplane_update = true;     // Reset flag

        // Notify Task_Enemy to start generating enemy squares
        xTaskNotifyGive(Task_Enemy_Handle);

        // Wait indefinitely until a message is received
        xQueueReceive(Queue_Airplane_Fight, &message, portMAX_DELAY);

        // Launch the ball if haven't yet when S1 is pressed during a game
        if(!LAUNCHED && S1_PRESSED)
        {
            LAUNCHED = true;
        }

        // Recored the previous position of the ball for clearing before move
        ball_prev_x = ball_x;
        ball_prev_y = ball_y;

        // Examine the message received, move accordingly without passing boarders
        switch(message){
            case BAR_CMD_LEFT:
            {
                // If boarder is met, set flag and not move
                if(airplane_x - 1 < 7 + airplaneWidthPixels / 2)
                    airplane_update = false;
                else
                    airplane_x--;        // Otherwise, go Left by 1 pixel
                break;
            }
            case BAR_CMD_RIGHT:
            {
                // If boarder is met, set flag and not move
                if(airplane_x + 1 > 123 - airplaneWidthPixels / 2)
                    airplane_update = false;
                else
                    airplane_x++;        // Otherwise, go Right by 1 pixel
                break;
            }
            case BAR_CMD_DOWN:
            {
                // If boarder is met, set flag and not move
                if(airplane_y + 1 > 115)
                    airplane_update = false;
                else
                    airplane_y++;        // Otherwise, go Down by 1 pixel
                break;
            }
            case BAR_CMD_UP:
            {
                // If boarder is met, set flag and not move
                if(airplane_y - 1 < 80)
                    airplane_update = false;
                else
                    airplane_y--;        // Otherwise, go Down by 1 pixel
                break;
            }
            case BAR_CMD_CENTER:        // Do not update the airplane on LCD if the board is held horizontally
            {
                airplane_update = false;
                break;
            }
        }

        if(!LAUNCHED)   // If the ball was not launched
        {
            // Make the ball follow the airplane's position, if it will be uodated
            if(airplane_update)
            {
                ball_reset();
            }
        }
        else
        {
            // Attempt to move the ball twice as fast as the airplane does
            ball_y -= 2;

            // If the ball is still in the range of the LCD, move it
            if(ball_y > 3)
            {
                lcd_draw_image(
                        ball_prev_x,
                        ball_prev_y,
                        ballWidthPixels,
                        ballHeightPixels,
                        image_ball,
                        LCD_COLOR_BLACK,
                        LCD_COLOR_BLACK
                );
                lcd_draw_image(
                        ball_x,
                        ball_y,
                        ballWidthPixels,
                        ballHeightPixels,
                        image_ball,
                        ball_lcd_color,
                        LCD_COLOR_BLACK
                );
            }
            else    // Otherwise, reset the ball
            {
                ball_reset();
            }
        }

        // Redraw the airplane on LCD, if needed
        if(airplane_update)
        {
            lcd_draw_image(
                    airplane_x,
                    airplane_y,
                    airplaneWidthPixels,
                    airplaneHeightPixels,
                    airplaneBitmaps,
                    LCD_COLOR_BLUE,
                    LCD_COLOR_BLACK
            );
        }

        // Reset S1 on MKII
        S1_PRESSED = false;

        // Delay for 20mS
        vTaskDelay(pdMS_TO_TICKS(20));
    }
}

/******************************************************************************
 * Helper method to reset the ball according to the current position of the
 * airplane, the ball will be placed 1 pixel above the airplane, centered at
 * the same x axis, respectively.
 ******************************************************************************/
void ball_reset(void)
{
    LAUNCHED = false;       // Reset the flag

    // Clear the previous image of the ball
    lcd_draw_image(
            ball_x,
            ball_y,
            ballWidthPixels,
            ballHeightPixels,
            image_ball,
            LCD_COLOR_BLACK,
            LCD_COLOR_BLACK
    );

    // Update the position of the ball according to the airplane
    ball_x = airplane_x;
    ball_y = airplane_y - 20;

    // Redraw the ball
    lcd_draw_image(
            ball_x,
            ball_y,
            ballWidthPixels,
            ballHeightPixels,
            image_ball,
            ball_lcd_color,
            LCD_COLOR_BLACK
    );
}

