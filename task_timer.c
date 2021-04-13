/*
 * task_timer.c
 *
 *  Created on: Apr 10, 2021
 *      Author: Justin Qiao
 */

#include <main.h>

TaskHandle_t Task_Accelerometer_Timer_Handle;

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

    // If S1 is being pressed, set the LSBit of debounce_state to a 1;
    if(ece353_MKII_S1())
    {
        debounce_state |= 0x01;
    }

    // If the de-bounce variable is equal to 0x7F, change the color of the tri-color LED.
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

    // If S1 is being pressed, set the LSBit of debounce_state to a 1;
    if(ece353_MKII_S2())
    {
        debounce_state |= 0x01;
    }

    // If the de-bounce variable is equal to 0x7F, change the color of the tri-color LED.
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
 ******************************************************************************/
void Task_Timer(void *pvParameters)
{
    static int counter = 0;

    // Timer goes 5mS per iteration
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
            // If the ball is not launched when S2 is pressed, change color
            if(!LAUNCHED)
            {
                ball_color = (ball_color + 1) % 11;
                ball_lcd_color = get_lcd_color(ball_color);
                S2_PRESSED = false;

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
        }

        // Generate an enemy square every 1.5 second
        if(counter++ == 300)
        {
            GENERATE = true;
            counter = 0;
        }

        // Wait for 5mS after each ADC Conversion
        vTaskDelay(pdMS_TO_TICKS(5));
    }
}


