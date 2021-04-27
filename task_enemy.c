/*
 * task_enemy.c
 *
 *  Created on: Apr 10, 2021
 *      Author: Justin Qiao
 */

#include <main.h>

TaskHandle_t Task_Enemy_Handle;

Enemy_t enemies[8][9];     // Matrix stores information about all enemy squares, restricted to this file

/******************************************************************************
 * This function resets the enemies. It will be called in reset_game().
 ******************************************************************************/
void enemy_reset()
{
    int i, j;       // Loop variables

    // Clear the matrix
    for(i = 0; i < 8; i++)
        for(j = 0; j < 9; j++)
            enemies[i][j].occupied = false;

    // Setup random seed
    srand(time(NULL));
}

/******************************************************************************
 * This task manages the random generations and breaks of enemy squares.
 * Each enemy square will have a Width = Height = 12.
 ******************************************************************************/
void Task_Enemy(void *pvParameters)
{
    int i, j;       // Loop variables
    int r, c;       // Temp variables, Row and Column indexes
    int x, y;       // Temp variables, X and Y coordinates

    int tank_r, tank_c;     // Store indexes of the tank
    int ball_r, ball_c;     // Store indexes of the ball

    // Temp variables, store draw frame information of different images
    // (x0, y0) is the left upper corner of an image
    // (x1, y1) is the right lower corner of an image
    int x0, x1, y0, y1;

    while(1)
    {
        // Wait until a game is started to manage enemy squares
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        // Try to generate a new enemy square
        if(GENERATE)
        {
            // Calculate indexes of the tank from its coordinates
            tank_r = (tank_y - 32) / 12;
            tank_c = (tank_x - 16) / 12;

            // Calculate indexes of the ball from its coordinates
            ball_r = (ball_y - 32) / 12;
            ball_c = (ball_x - 16) / 12;

            GENERATE = false;       // Reset flag

            // Randomly generate row and column indexes
            c = rand() % 9;
            r = rand() % 8;

            // If the matrix has an empty spot at that place, put a new square in
            // A new square will NOT overlaps the tank nor the ball
            if(!enemies[r][c].occupied
                    && !((r >= tank_r - 2) && (r <= tank_r + 2) && (c >= tank_c - 2) && (c <= ball_c + 2))
                    && !((r >= ball_r - 1) && (r <= ball_r + 1) && (c >= ball_c - 1) && (c <= ball_c + 1)))
            {
                enemies[r][c].occupied = true;      // Set flag in matrix

                // Calculate coordinates of the new square from indexes
                x = c * 12 + 16;
                y = r * 12 + 32;

                // Randomly generate a color for the new square
                Color_t color = rand() % 11;
                enemies[r][c].color = color;
                uint16_t lcd_color = get_lcd_color(color);

                // Get the current draw frame of the square being generated
                lcd_get_draw_frame(x, y, 12, 12, &x0, &x1, &y0, &y1);

                // Update the occupied matrix defined in task_breaker
                for(i = y0; i <= y1; i++)
                {
                    for(j = x0; j <= x1; j++)
                    {
                        occupied[i][j] = true;
                    }
                }

                // draw the new square
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
        lcd_get_draw_frame(ball_x, ball_y, ballWidthPixels, ballHeightPixels, &x0, &x1, &y0, &y1);

        // Update the squares accordingly to the direction of the ball
        if(ball_launched)
        {
            switch(ball_dir)
            {
                case 0:     // Moving to the Left
                {
                    check_squares(x0, y0, x0, y1);

                    break;
                }
                case 1:     // Moving to the Right
                {
                    check_squares(x1, y0, x1, y1);

                    break;
                }
                case 2:     // Moving Upward
                {
                    check_squares(x0, y0, x1, y0);

                    break;
                }
                case 3:     // Moving Downward
                {
                    check_squares(x0, y1, x1, y1);

                    break;
                }
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
void check_squares(int xk_1, int yk_1, int xk_2, int yk_2)
{
    int ball_r_1, ball_c_1, ball_r_2, ball_c_2;     // Indexes of the given coordinates
    int square_x_1, square_y_1, square_x_2, square_y_2;     // Coordinates of the two squares being checked
    int i, j;       // Loop variables
    Color_t color;      // The randomly re-chosen color

    // Temp variables, store draw frame information of different images
    // (x0, y0) is the left upper corner of an image
    // (x1, y1) is the right lower corner of an image
    int x0, x1, y0, y1;

    // Flag variables, true when a square is broken, false otherwise
    bool match_1 = false;
    bool match_2 = false;

    // Calculate indexes from the given coordinates, they may or may not be the same position
    ball_c_1 = (xk_1 - 16 + 5) / 12;
    ball_r_1 = (yk_1 - 32 + 6) / 12;
    ball_c_2 = (xk_2 - 16 + 5) / 12;
    ball_r_2 = (yk_2 - 32 + 6) / 12;

    // If the ball is launched and at least one of the squares it is traveling into is occupied, check the two given squares
    if(ball_launched && ((enemies[ball_r_1][ball_c_1].occupied && ball_r_1 < 8 && ball_c_1 < 9 ) || (enemies[ball_r_2][ball_c_2].occupied) && ball_r_2 < 8 && ball_c_2 < 9))
    {
        // Calculate the coordinates of the center of the two squares from indexes
        square_x_1 = ball_c_1 * 12 + 16;
        square_y_1 = ball_r_1 * 12 + 32;
        square_x_2 = ball_c_2 * 12 + 16;
        square_y_2 = ball_r_2 * 12 + 32;

        // Break square_1 if the color of the ball matches the color of the square_1
        if(enemies[ball_r_1][ball_c_1].occupied && enemies[ball_r_1][ball_c_1].color == tank_color)
        {
            // Reset flag so that the same square will not be rechecked
            enemies[ball_r_1][ball_c_1].occupied = false;

            // Get the current draw frame of the square being destroyed
            lcd_get_draw_frame(square_x_1, square_y_1, 12, 12, &x0, &x1, &y0, &y1);

            // Update the occupied matrix defined in task_breaker
            for(i = y0; i <= y1; i++)
            {
                for(j = x0; j <= x1; j++)
                {
                    occupied[i][j] = false;
                }
            }

            // Set flag showing that square 1 is broken
            match_1 = true;

            // Each time a square is destroyed, get 1 point
            score++;
        }

        // Break square_2 if the color of the ball matches the color of the square_2
        if(enemies[ball_r_2][ball_c_2].occupied && enemies[ball_r_2][ball_c_2].color == tank_color)
        {
            // Reset flag
            enemies[ball_r_2][ball_c_2].occupied = false;

            // Get the current draw frame of the square being destroyed
            lcd_get_draw_frame(square_x_2, square_y_2, 12, 12, &x0, &x1, &y0, &y1);

            // Update the occupied matrix defined in task_breaker
            for(i = y0; i <= y1; i++)
            {
                for(j = x0; j <= x1; j++)
                {
                    occupied[i][j] = false;
                }
            }

            // Set flag showing that square 2 is broken
            match_2 = true;

            // Each time a square is destroyed, get 1 point
            score++;
        }

        ball_reset();       // Reset the ball

        xSemaphoreTake(Sem_LCD, portMAX_DELAY);

        // Clear square_1, if needed
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

        // Clear square_2, if needed
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

        // Randomly choose a new color for square_1 if it was color-mismatched
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

        // Randomly choose a new color for square_2 if it was color-mismatched
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

        if(match_1 || match_2)
        {
            // Update score board
            update_score();
        }

        music_play_hit();       // Play hit music

        led_blink();        // Blink LED on MKII three times indicating a hit
    }
}
