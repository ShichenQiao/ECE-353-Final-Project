/*
 * task_enemy.c
 *
 *  Created on: Apr 10, 2021
 *      Author: Justin Qiao
 */

#include <main.h>

TaskHandle_t Task_Enemy_Handle;

Enemy_t enemies[8][9];     // Matrix stores information about all enemy squares

/******************************************************************************
 * This task manages the random generations and breaks of enemy squares.
 * Each enemy square will have a Width = Height = 12
 ******************************************************************************/
void Task_Enemy(void *pvParameters)
{
    int i, j;       // Loop variables
    int r, c;       // Temp variables, Row and Column indexes
    int x, y;       // Temp variables, X and Y coordinates
    int ball_r, ball_c;     // The row and column the ball is in, if any

    // Clear the matrix
    for(i = 0; i < 8; i++)
        for(j = 0; j < 9; j++)
            enemies[i][j].occupied = false;

    // Setup random seed
    srand(time(NULL));

    while(1)
    {
        // Wait until a game is started to manage enemy squares
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        // Try to generate a new enemy square
        if(GENERATE)
        {
            GENERATE = false;       // Reset flag

            // Randomly generate row and column indexes
            c = rand() % 9;
            r = rand() % 8;

            // If the matrix has an empty spot at that place, put a new square in
            if(!enemies[r][c].occupied)
            {
                enemies[r][c].occupied = true;      // Set flag

                // Calculate coordinates from indexes
                x = c * 12 + 16;
                y = r * 12 + 32;

                // Randomly generate a color for the new square
                Color_t color = rand() % 11;
                enemies[r][c].color = color;

                // Draw the new square
                uint16_t lcd_color = get_lcd_color(color);

                xSemaphoreTake(Sem_LCD, portMAX_DELAY);

                lcd_draw_rectangle(
                    x,
                    y,
                    12,
                    12,
                    lcd_color
                );

                xSemaphoreGive(Sem_LCD);

                for(i = y - 6; i < y + 6; i++)
                {
                    for(j = x - 7; j < x + 5; j++)
                    {
                        occupied[i][j] = true;
                    }
                }
            }
        }

        // Get the current draw frame of the ball
        int x0, x1, y0, y1;
        get_draw_frame(ball_x, ball_y, ballWidthPixels, ballHeightPixels, &x0, &x1, &y0, &y1);

        // Calculate indexes from coordinates
        ball_c = (ball_x - 16) / 12;
        ball_r = (ball_y - 32) / 12;

        switch(ball_dir)
        {
            case 0:
            {
                ball_c = (x0 - 16) / 12;
                ball_r = (y0 - 32) / 12;
                if(check_square(ball_r, ball_c) != 1)
                {
                    ball_c = (x0 - 16) / 12;
                    ball_r = (y1 - 32) / 12;

                    check_square(ball_r, ball_c);

                }
                break;
            }
            case 1:
            {
                ball_c = (x1 - 16) / 12;
                ball_r = (y0 - 32) / 12;
                if(check_square(ball_r, ball_c != 1))
                {
                    ball_c = (x1 - 16) / 12;
                    ball_r = (y1 - 32) / 12;

                    check_square(ball_r, ball_c);

                }
                break;
            }
            case 2:
            {
                ball_c = (x0 - 16) / 12;
                ball_r = (y0 - 32) / 12;
                if(check_square(ball_r, ball_c) != 1)
                {
                    ball_c = (x1 - 16) / 12;
                    ball_r = (y0 - 32) / 12;

                    check_square(ball_r, ball_c);

                }
                break;
            }
            case 3:
            {
                ball_c = (x0 - 16) / 12;
                ball_r = (y1 - 32) / 12;
                if(check_square(ball_r, ball_c) != 1)
                {
                    ball_c = (x1 - 16) / 12;
                    ball_r = (y1 - 32) / 12;

                    check_square(ball_r, ball_c);

                }
                break;
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

// -1 for no collision, 0 for failed to break, 1 for broke square
int check_square(int ball_r, int ball_c)
{
    int x, y;
    int i, j;
    if(ball_launched && enemies[ball_r][ball_c].occupied)
    {
        // Clear the square if the color of the ball matches the color of the square
        if(enemies[ball_r][ball_c].color == ball_color)
        {
            // Reset flag
            enemies[ball_r][ball_c].occupied = false;

            // Calculate coordinates from indexes
            x = ball_c * 12 + 16;
            y = ball_r * 12 + 32;

            for(i = y - 6; i < y + 6; i++)
            {
                for(j = x - 7; j < x + 5; j++)
                {
                    occupied[i][j] = false;
                }
            }

            xSemaphoreTake(Sem_LCD, portMAX_DELAY);

            // Clear the square
            lcd_draw_rectangle(
                x,
                y,
                12,
                12,
                LCD_COLOR_BLACK
            );

            xSemaphoreGive(Sem_LCD);

            // Reset the ball if color matches
            ball_reset();

            return 1;
        }

        ball_reset();

        return 0;
    }
    return -1;
}
