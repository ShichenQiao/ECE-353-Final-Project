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

    int tank_r, tank_c;
    int ball_r, ball_c;

    int x0, x1, y0, y1;     // Used to store draw frame information of a picture

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
            tank_r = (tank_y - 32) / 12;
            tank_c = (tank_x - 16) / 12;
            ball_r = (ball_y - 32) / 12;
            ball_c = (ball_x - 16) / 12;

            GENERATE = false;       // Reset flag

            // Randomly generate row and column indexes
            c = rand() % 9;
            r = rand() % 8;

            // If the matrix has an empty spot at that place, put a new square in. Made sure that a new square will NOT overlaps the tank
            if(!enemies[r][c].occupied && !((r >= tank_r - 2) && (r <= tank_r + 2) && (c >= tank_c - 2) && (c <= ball_c + 2)) && !((r >= ball_r - 1) && (r <= ball_r + 1) && (c >= ball_c - 1) && (c <= ball_c + 1)))
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

                // Get the current draw frame of the square being generated
                get_draw_frame(x, y, 12, 12, &x0, &x1, &y0, &y1);

                for(i = y0; i <= y1; i++)
                {
                    for(j = x0; j <= x1; j++)
                    {
                        occupied[i][j] = true;
                    }
                }

                xSemaphoreTake(Sem_LCD, portMAX_DELAY);

                lcd_draw_rectangle(
                    x,
                    y,
                    12,
                    12,
                    lcd_color
                );

                xSemaphoreGive(Sem_LCD);
            }
        }

        // Get the current draw frame of the ball
        get_draw_frame(ball_x, ball_y, ballWidthPixels, ballHeightPixels, &x0, &x1, &y0, &y1);

        switch(ball_dir)
        {
            case 0:
            {
                check_squares(x0, y0, x0, y1);

                break;
            }
            case 1:
            {
                check_squares(x1, y0, x1, y1);

                break;
            }
            case 2:
            {
                check_squares(x0, y0, x1, y0);

                break;
            }
            case 3:
            {
                check_squares(x0, y1, x1, y1);

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
void check_squares(int xk_1, int yk_1, int xk_2, int yk_2)
{
    int square_x_1, square_y_1, square_x_2, square_y_2;
    int x0, x1, y0, y1;
    int i, j;
    int ball_r_1, ball_c_1, ball_r_2, ball_c_2;
    Color_t color;

    bool match_1 = false;
    bool match_2 = false;

    // Calculate indexes from coordinates
    ball_c_1 = (xk_1 - 16 + 5) / 12;
    ball_r_1 = (yk_1 - 32 + 6) / 12;
    ball_c_2 = (xk_2 - 16 + 5) / 12;
    ball_r_2 = (yk_2 - 32 + 6) / 12;

    if(ball_launched && ((enemies[ball_r_1][ball_c_1].occupied && ball_r_1 < 8 && ball_c_1 < 9 ) || (enemies[ball_r_2][ball_c_2].occupied) && ball_r_2 < 8 && ball_c_2 < 9))
    {
        // Calculate coordinates from indexes
        square_x_1 = ball_c_1 * 12 + 16;
        square_y_1 = ball_r_1 * 12 + 32;
        square_x_2 = ball_c_2 * 12 + 16;
        square_y_2 = ball_r_2 * 12 + 32;

        // Clear the square if the color of the ball matches the color of the square
        if(enemies[ball_r_1][ball_c_1].occupied && enemies[ball_r_1][ball_c_1].color == ball_color)
        {
            // Reset flag
            enemies[ball_r_1][ball_c_1].occupied = false;

            // Get the current draw frame of the square being destroyed
            get_draw_frame(square_x_1, square_y_1, 12, 12, &x0, &x1, &y0, &y1);

            for(i = y0; i <= y1; i++)
            {
                for(j = x0; j <= x1; j++)
                {
                    occupied[i][j] = false;
                }
            }

            match_1 = true;
        }

        // Clear the square if the color of the ball matches the color of the square
        if(enemies[ball_r_2][ball_c_2].occupied && enemies[ball_r_2][ball_c_2].color == ball_color)
        {
            // Reset flag
            enemies[ball_r_2][ball_c_2].occupied = false;

            // Get the current draw frame of the square being destroyed
            get_draw_frame(square_x_2, square_y_2, 12, 12, &x0, &x1, &y0, &y1);

            for(i = y0; i <= y1; i++)
            {
                for(j = x0; j <= x1; j++)
                {
                    occupied[i][j] = false;
                }
            }

            match_2 = true;
        }

        ball_reset();

        xSemaphoreTake(Sem_LCD, portMAX_DELAY);

        if(match_1)
        {
            // Clear the square
            lcd_draw_rectangle(
                square_x_1,
                square_y_1,
                12,
                12,
                LCD_COLOR_BLACK
            );
        }

        if(match_2)
        {
            // Clear the square
            lcd_draw_rectangle(
                square_x_2,
                square_y_2,
                12,
                12,
                LCD_COLOR_BLACK
            );
        }

        if(!match_1 && enemies[ball_r_1][ball_c_1].occupied && ball_r_1 < 8 && ball_c_1 < 9)
        {
            // Randomly generate a color for the new square
            color = rand() % 11;
            enemies[ball_r_1][ball_c_1].color = color;

            // Draw the new square
            uint16_t lcd_color = get_lcd_color(color);

            lcd_draw_rectangle(
                square_x_1,
                square_y_1,
                12,
                12,
                lcd_color
            );
        }

        if(!match_2 && enemies[ball_r_2][ball_c_2].occupied && ball_r_2 < 8 && ball_c_2 < 9)
        {
            // Randomly generate a color for the new square
            color = rand() % 11;
            enemies[ball_r_2][ball_c_2].color = color;

            // Draw the new square
            uint16_t lcd_color = get_lcd_color(color);

            lcd_draw_rectangle(
                square_x_2,
                square_y_2,
                12,
                12,
                lcd_color
            );
        }

        xSemaphoreGive(Sem_LCD);
    }
}
