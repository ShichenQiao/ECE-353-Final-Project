/*
 * task_breaker.c
 *
 *  Created on: Apr 10, 2021
 *      Author: Justin Qiao
 */

#include <main.h>

#define QUEUE_LEN  2

TaskHandle_t Task_Breaker_Handle;
QueueHandle_t Queue_Breaker;
SemaphoreHandle_t Sem_LCD;

// Shared information about the ball
uint8_t ball_x;
uint8_t ball_y;
Color_t ball_color;
uint16_t ball_lcd_color;
bool ball_launched = false;      // Flag variable, true if the ball has been launched
int ball_dir = 2;   // 0 for LEFT, 1 for RIGHT, 2 for UP, and 3 for DOWN

// Shared coordinates of the tank
uint8_t tank_x;
uint8_t tank_y;
int tank_dir = 2;   // 0 for LEFT, 1 for RIGHT, 2 for UP, and 3 for DOWN

bool occupied[LCD_VERTICAL_MAX][LCD_HORIZONTAL_MAX];    // Matrix to record whether a pixel on the LCD is occupied

/******************************************************************************
 * This function will initialize Queue_Breaker, initialize the LCD and initialize
 * Sem_LCD.
 ******************************************************************************/
void Task_Breaker_Init(void)
{
    Queue_Breaker = xQueueCreate(QUEUE_LEN, sizeof(BAR_CMD_t));

    Crystalfontz128x128_Init();

    Sem_LCD = xSemaphoreCreateBinary();
    xSemaphoreGive(Sem_LCD);
}

/******************************************************************************
 * This is the main task controlling the gaming mode. Commands can be sent to
 * Queue_Breaker to be processed.
 ******************************************************************************/
void Task_Breaker(void *pvParameters)
{
    // Initialize position of the tank
    tank_x = 67;
    tank_y = 123;

    // Flag variable, the tank shown on LCD will only updates if this variable is true
    bool tank_update = true;

    // Initialize position and color of the ball
    ball_x = tank_x;
    ball_y = tank_y;
    ball_color = GREEN;
    ball_lcd_color = get_lcd_color(ball_color);

    // Helper variables to recored previous position of the ball right before LCD updates
    uint8_t ball_prev_x;
    uint8_t ball_prev_y;

    int i, j;   // Loop variables

    // Setup occupied matrix
    for(i = 0; i < LCD_VERTICAL_MAX; i++)
    {
        for(j = 0; j < LCD_HORIZONTAL_MAX; j++)
        {
            // Any pixel outside the battle field boarders are considered occupied
            if(i < 25 || i > 129 || j < 7 || j > 124)
            {
                occupied[i][j] = true;
            }
            else
            {
                occupied[i][j] = false;
            }
        }
    }

    xSemaphoreTake(Sem_LCD, portMAX_DELAY);

    // Draw initial image of the tank
    lcd_draw_image(
            tank_x,
            tank_y,
            tankWidthPixels,
            tankHeightPixels,
            tank_upBitmaps,
            ball_lcd_color,
            LCD_COLOR_BLACK
    );

    // Draw boarders on LCD
    lcd_draw_rectangle(4, 67, 5, 128, LCD_COLOR_RED);       // Left boarder
    lcd_draw_rectangle(127, 67, 5, 128, LCD_COLOR_RED);       // Right boarder
    lcd_draw_rectangle(67, 22, 128, 5, LCD_COLOR_RED);       // Upper boarder

    xSemaphoreGive(Sem_LCD);

    // What until S1 is pressed to start a game
    while(!S1_PRESSED){};
    S1_PRESSED = false;

    // Enter the Gaming Mode
    while(1)
    {
        BAR_CMD_t message;     // Message received from Queue_Breaker

        tank_update = true;     // Reset flag

        // Notify Task_Enemy to start generating enemy squares
        xTaskNotifyGive(Task_Enemy_Handle);

        xTaskNotifyGive(Task_Score_Board_Handle);

        // Wait indefinitely until a message is received
        xQueueReceive(Queue_Breaker, &message, portMAX_DELAY);

        // Get the current draw frame of the tank
        int x0, x1, y0, y1;
        get_draw_frame(tank_x, tank_y, tankWidthPixels, tankHeightPixels, &x0, &x1, &y0, &y1);

        // Examine the message received, move accordingly without passing boarders
        switch(message){
            case BAR_CMD_LEFT:
            {
                for(i = y0; i < y1; i++)
                {
                    if(occupied[i][x0 - 1])
                    {
                        tank_update = false;
                    }
                }

                tank_dir = 0;

                if(tank_update)
                {
                    tank_x--;        // Otherwise, go Left by 1 pixel

                    xSemaphoreTake(Sem_LCD, portMAX_DELAY);

                    lcd_draw_image(
                            tank_x,
                            tank_y,
                            tankWidthPixels,
                            tankHeightPixels,
                            tank_leftBitmaps,
                            ball_lcd_color,
                            LCD_COLOR_BLACK
                    );

                    xSemaphoreGive(Sem_LCD);
                }
                break;
            }
            case BAR_CMD_RIGHT:
            {
                for(i = y0; i < y1; i++)
                {
                    if(occupied[i][x1 + 1])
                    {
                        tank_update = false;
                    }
                }

                tank_dir = 1;

                if(tank_update)
                {
                    tank_x++;        // Otherwise, go Right by 1 pixel

                    xSemaphoreTake(Sem_LCD, portMAX_DELAY);

                    lcd_draw_image(
                            tank_x,
                            tank_y,
                            tankWidthPixels,
                            tankHeightPixels,
                            tank_rightBitmaps,
                            ball_lcd_color,
                            LCD_COLOR_BLACK
                    );

                    xSemaphoreGive(Sem_LCD);
                }
                break;
            }
            case BAR_CMD_DOWN:
            {
                for(i = x0; i < x1; i++)
                {
                    if(occupied[y1 + 1][i])
                    {
                        tank_update = false;
                    }
                }

                tank_dir = 3;

                if(tank_update)
                {
                    tank_y++;        // Otherwise, go Down by 1 pixel

                    xSemaphoreTake(Sem_LCD, portMAX_DELAY);

                    lcd_draw_image(
                            tank_x,
                            tank_y,
                            tankWidthPixels,
                            tankHeightPixels,
                            tank_downBitmaps,
                            ball_lcd_color,
                            LCD_COLOR_BLACK
                    );

                    xSemaphoreGive(Sem_LCD);
                }
                break;
            }
            case BAR_CMD_UP:
            {
                for(i = x0; i < x1; i++)
                {
                    if(occupied[y0 - 1][i])
                    {
                        tank_update = false;
                    }
                }

                tank_dir = 2;

                if(tank_update)
                {
                    tank_y--;        // Otherwise, go Down by 1 pixel

                    xSemaphoreTake(Sem_LCD, portMAX_DELAY);

                    lcd_draw_image(
                            tank_x,
                            tank_y,
                            tankWidthPixels,
                            tankHeightPixels,
                            tank_upBitmaps,
                            ball_lcd_color,
                            LCD_COLOR_BLACK
                    );

                    xSemaphoreGive(Sem_LCD);
                }
                break;
            }
            case BAR_CMD_CENTER:        // Do not update the tank on LCD if the board is held horizontally
            {
                tank_update = false;
                break;
            }
        }

        // Launch the ball if haven't yet when S1 is pressed during a game
        if(!ball_launched && S1_PRESSED)
        {
            ball_dir = tank_dir;
            ball_launched = true;
        }

        // Recored the previous position of the ball for clearing before move
        ball_prev_x = ball_x;
        ball_prev_y = ball_y;

        if(!ball_launched)   // If the ball was not launched
        {
            // Make the ball follow the tank's position, if it will be updated
            if(tank_update)
            {
                ball_reset();
            }
        }
        else
        {
            // If the ball is still in the range of the boarders, move it
            if(ball_y > 32 && ball_y < 125 && ball_x > 13 && ball_x < 119)
            {
                // Attempt to move the ball twice as fast as the tank does
                switch(ball_dir)
                {
                    case 0:
                    {
                        ball_x -= 2;
                        break;
                    }
                    case 1:
                    {
                        ball_x += 2;
                        break;
                    }
                    case 2:
                    {
                        ball_y -= 2;
                        break;
                    }
                    case 3:
                    {
                        ball_y += 2;
                        break;
                    }
                }

                xSemaphoreTake(Sem_LCD, portMAX_DELAY);

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

                xSemaphoreGive(Sem_LCD);

                tank_recover();
            }
            else    // Otherwise, reset the ball
            {
                ball_reset();
            }
        }

        // Reset S1 on MKII
        S1_PRESSED = false;

        // Delay for 20mS
        vTaskDelay(pdMS_TO_TICKS(20));
    }
}

/******************************************************************************
 * Helper method to reset the ball according to the current position of the
 * tank, the ball will have the same center as the tank, respectively.
 ******************************************************************************/
void ball_reset(void)
{
    xSemaphoreTake(Sem_LCD, portMAX_DELAY);

    ball_launched = false;       // Reset the flag

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

    xSemaphoreGive(Sem_LCD);

    tank_recover();

    // Update the position of the ball according to the tank
    ball_x = tank_x;
    ball_y = tank_y;
}

void tank_recover()
{
    xSemaphoreTake(Sem_LCD, portMAX_DELAY);

    // Redraw the tank at its current position
    switch(tank_dir)
    {
        case 0:
        {
            lcd_draw_image(
                    tank_x,
                    tank_y,
                    tankWidthPixels,
                    tankHeightPixels,
                    tank_leftBitmaps,
                    ball_lcd_color,
                    LCD_COLOR_BLACK
            );
            break;
        }
        case 1:
        {
            lcd_draw_image(
                    tank_x,
                    tank_y,
                    tankWidthPixels,
                    tankHeightPixels,
                    tank_rightBitmaps,
                    ball_lcd_color,
                    LCD_COLOR_BLACK
            );
            break;
        }
        case 2:
        {
            lcd_draw_image(
                    tank_x,
                    tank_y,
                    tankWidthPixels,
                    tankHeightPixels,
                    tank_upBitmaps,
                    ball_lcd_color,
                    LCD_COLOR_BLACK
            );
            break;
        }
        case 3:
        {
            lcd_draw_image(
                    tank_x,
                    tank_y,
                    tankWidthPixels,
                    tankHeightPixels,
                    tank_downBitmaps,
                    ball_lcd_color,
                    LCD_COLOR_BLACK
            );
            break;
        }
    }

    xSemaphoreGive(Sem_LCD);
}

void get_draw_frame(int x, int y, int image_width_pixels, int image_height_pixels, int *x0, int *x1, int *y0, int *y1)
{
    *x0 = x - (image_width_pixels/2);
    *x1 = x + (image_width_pixels/2);
    if( (image_width_pixels & 0x01) == 0x00)
    {
      *x1--;
    }

    *y0 = y  - (image_height_pixels/2);
    *y1 = y  + (image_height_pixels/2) ;
    if( (image_height_pixels & 0x01) == 0x00)
    {
      *y1--;
    }
}

