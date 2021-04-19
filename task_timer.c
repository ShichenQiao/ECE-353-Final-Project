/*
 * task_timer.c
 *
 *  Created on: Apr 10, 2021
 *      Author: Justin Qiao
 */

#include <main.h>

TaskHandle_t Task_Timer_Handle;

// Flag variables
volatile bool S1_PRESSED = false;
volatile bool S2_PRESSED = false;
volatile bool GENERATE = false;

/******************************************************************************
 * De-bounce switch S1 on MKII.
 *****************************************************************************/
bool debounce_s1(void)
{
    static uint8_t debounce_state = 0x00;

    // Shift the de-bounce variable to the left
    debounce_state = debounce_state << 1;

    // If S1 is being pressed, set the LSBit of debounce_state to a 1
    if(ece353_MKII_S1())
    {
        debounce_state |= 0x01;
    }

    // If the de-bounce variable is equal to 0x7F, return true
    if(debounce_state == 0x7F)
    {
        return true;
    }
    else
    {
        return false;
    }

}

/******************************************************************************
 * De-bounce switch S2 on MKII.
 *****************************************************************************/
bool debounce_s2(void)
{
    static uint8_t debounce_state = 0x00;

    // Shift the de-bounce variable to the left
    debounce_state = debounce_state << 1;

    // If S1 is being pressed, set the LSBit of debounce_state to a 1
    if(ece353_MKII_S2())
    {
        debounce_state |= 0x01;
    }

    // If the de-bounce variable is equal to 0x7F, return true
    if(debounce_state == 0x7F)
    {
        return true;
    }
    else
    {
        return false;
    }

}

/******************************************************************************
 * Shared timer between all tasks.
 * Initiate an ADC14 conversion every 5mS.
 * Check S1, and S2 states every 5mS.
 * When S2 is pressed during a active game and the ball is not launched, change
 * the color of the tank.
 * Send GENERATE signal to task_enemy every 2 seconds.
 ******************************************************************************/
void Task_Timer(void *pvParameters)
{
    static int counter = 0;     // Used to count for 2 seconds (5mS * 400 = 2S)

    // Timer goes roughly 5mS per iteration
    while(1){
        // Start an ADC14 conversion every 5mS
        ADC14->CTL0 |= ADC14_CTL0_SC | ADC14_CTL0_ENC;

        // Debounce and set S1 state
        if(!S1_PRESSED && debounce_s1())
        {
            S1_PRESSED = true;
        }

        // Debounce and set S2 state
        if(!S1_PRESSED && debounce_s2())
        {
            S2_PRESSED = true;
            // If the ball is not launched during a game when S2 is pressed, change color
            if(game_on_going && !ball_launched)
            {
                // Intentionally mixed enum type and int to rotate between colors
                tank_color = (tank_color + 1) % 11;
                tank_lcd_color = get_lcd_color(tank_color);

                xSemaphoreTake(Sem_LCD, portMAX_DELAY);

                // Change color of the tank
                switch(tank_dir)
                {
                    case 0:     // Facing Left
                    {
                        lcd_draw_image(
                                tank_x,
                                tank_y,
                                tankWidthPixels,
                                tankHeightPixels,
                                tank_leftBitmaps,
                                tank_lcd_color,
                                LCD_COLOR_BLACK
                        );
                        break;
                    }
                    case 1:     // Facing Right
                    {
                        lcd_draw_image(
                                tank_x,
                                tank_y,
                                tankWidthPixels,
                                tankHeightPixels,
                                tank_rightBitmaps,
                                tank_lcd_color,
                                LCD_COLOR_BLACK
                        );
                        break;
                    }
                    case 2:     // Facing Up
                    {
                        lcd_draw_image(
                                tank_x,
                                tank_y,
                                tankWidthPixels,
                                tankHeightPixels,
                                tank_upBitmaps,
                                tank_lcd_color,
                                LCD_COLOR_BLACK
                        );
                        break;
                    }
                    case 3:     // Facing Down
                    {
                        lcd_draw_image(
                                tank_x,
                                tank_y,
                                tankWidthPixels,
                                tankHeightPixels,
                                tank_downBitmaps,
                                tank_lcd_color,
                                LCD_COLOR_BLACK
                        );
                        break;
                    }
                }

                xSemaphoreGive(Sem_LCD);

                // Reset S2 flag
                S2_PRESSED = false;
            }
        }

        // Send a GENERATE signal to task_enemy every 2 seconds
        if(counter++ == 400)
        {
            GENERATE = true;
            counter = 0;
        }

        // Wait for 5mS after each iteration
        vTaskDelay(pdMS_TO_TICKS(5));
    }
}


