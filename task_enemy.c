/*
 * task_enemy.c
 *
 *  Created on: Apr 10, 2021
 *      Author: Justin Qiao
 */

#include <main.h>

TaskHandle_t Task_Enemy_Handle;

/******************************************************************************
 * This task manages the random generations and breaks of enemy squares.
 ******************************************************************************/
void Task_Enemy(void *pvParameters)
{
    Enemy_t enemies[4][11];     // Matrix stores information about all enemy squares
    int i, j;       // Loop variables
    int r, c;       // Temp variables, Row and Column indexes
    int x, y;       // Temp variables, X and Y coordinates
    int ball_r, ball_c;     // The row and column the ball is in, if any
    int left, right;        // X coordinates of the left and right edges of the ball

    // Clear the matrix
    for(i = 0; i < 4; i++)
        for(j = 0; j < 11; j++)
            enemies[i][j].occupied = false;

    while(1)
    {
        // Wait until a game is started to manage enemy squares
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        // If the ball is in the area where an enemy square can be at
        if(ball_y < 70)
        {
            // Calculate indexes from coordinates
            ball_r = ball_y / 10 - 3;
            ball_c = ball_x / 10 - 1;

            // Calculate coordinates of the edges of the ball
            left = ball_x - ballWidthPixels / 2 - 1;
            right = ball_x + ballWidthPixels / 2 - 1;

            // If the ball's upper bond's center reaches a square that is occupied
            if(enemies[ball_r][ball_c].occupied)
            {
                // Clear the square if the color of the ball matches the color of the square
                if(enemies[ball_r][ball_c].color == ball_color)
                {
                    // Reset flag
                    enemies[ball_r][ball_c].occupied = false;

                    // Calculate coordinates from indexes
                    x = ball_c * 10 + 16;
                    y = ball_r * 10 + 30;

                    xSemaphoreTake(Sem_LCD, portMAX_DELAY);

                    // Clear the square
                    lcd_draw_rectangle(
                        x,
                        y,
                        10,
                        10,
                        LCD_COLOR_BLACK
                    );

                    xSemaphoreGive(Sem_LCD);
                }

                // Reset the ball no matter color matches or not
                ball_reset();
            }
            else{       // Otherwise, check the other square that the ball might be in
                // Check whether the ball is also inside the square on the left or on the right
                if(ball_c > left / 10 - 1)
                    ball_c = left / 10 - 1;
                else if(ball_c < right / 10 - 1)
                    ball_c = right / 10 - 1;

                // Repeat to examine a collision for the square we are interested in if its occupied
                if(enemies[ball_r][ball_c].occupied)
                {
                    // Clear the square if the color of the ball matches the color of the square
                    if(enemies[ball_r][ball_c].color == ball_color)
                    {
                        // Reset flag
                        enemies[ball_r][ball_c].occupied = false;

                        // Calculate coordinates from indexes
                        x = (ball_c) * 10 + 16;
                        y = ball_r * 10 + 30;

                        xSemaphoreTake(Sem_LCD, portMAX_DELAY);

                        // Clear the square
                        lcd_draw_rectangle(
                            x,
                            y,
                            10,
                            10,
                            LCD_COLOR_BLACK
                        );

                        xSemaphoreGive(Sem_LCD);
                    }

                    // Reset the ball no matter color matches or not
                    ball_reset();
                }

            }
        }
        else if(GENERATE)       // Try to generate a new enemy square only when the ball is not in zone
        {
            GENERATE = false;       // Reset flag

            // Randomly generate row and column indexes
            c = rand() % 11;
            r = rand() % 4;

            // If the matrix has an empty spot at that place, put a new square in
            if(!enemies[r][c].occupied)
            {
                enemies[r][c].occupied = true;      // Set flag

                // Calculate coordinates from indexes
                x = c * 10 + 16;
                y = r * 10 + 30;

                // Randomly generate a color for the new square
                Color_t color = rand() % 11;
                enemies[r][c].color = color;

                // Draw the new square
                uint16_t lcd_color = get_lcd_color(color);

                xSemaphoreTake(Sem_LCD, portMAX_DELAY);

                lcd_draw_rectangle(
                    x,
                    y,
                    10,
                    10,
                    lcd_color
                );

                xSemaphoreGive(Sem_LCD);
            }
        }
    }
}

/******************************************************************************
 * Helper method to convert a Color_t to uint16_t (LCD colors).
 ******************************************************************************/
uint16_t get_lcd_color(Color_t color)
{
    switch(color){
        case RED: return LCD_COLOR_RED;
        case GREEN: return LCD_COLOR_GREEN;
        case GREEN2: return LCD_COLOR_GREEN2;
        case BLUE: return LCD_COLOR_BLUE;
        case BLUE2: return LCD_COLOR_BLUE2;
        case YELLOW: return LCD_COLOR_YELLOW;
        case ORANGE: return LCD_COLOR_ORANGE;
        case CYAN: return LCD_COLOR_CYAN;
        case MAGENTA: return LCD_COLOR_MAGENTA;
        case GRAY: return LCD_COLOR_GRAY;
        case BROWN: return LCD_COLOR_BROWN;
        default: return 0;
    }
}
