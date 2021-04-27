/*
 * task_setting_page.c
 *
 *  Created on: Apr 27, 2021
 *      Author: Justin Qiao
 */

#include "main.h"

TaskHandle_t Task_Setting_Page_Handle;

/******************************************************************************
 *
 ******************************************************************************/
void Task_Setting_Page(void *pvParameters)
{
    while(1)
    {
        // Wait until entering setting mode from the home page
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        while(1)
        {
            lcd_print_char(49, 73, 'S');
            lcd_print_char(57, 73, 'E');
            lcd_print_char(65, 73, 'T');
            lcd_print_char(73, 73, 'T');
            lcd_print_char(81, 73, 'I');
            lcd_print_char(89, 73, 'N');
            lcd_print_char(97, 73, 'G');

            while(!S1_PRESSED && !S2_PRESSED){}

            if(S2_PRESSED)
            {
                S2_PRESSED = false;     // Reset S2

                // Cover setting page
                lcd_draw_rectangle(
                  67,
                  67,
                  132,
                  132,
                  LCD_COLOR_BLACK
                );

                // Exit Setting mode
                break;
            }
            else if(S1_PRESSED)
            {
                S1_PRESSED = false;     // Reset S1
            }
        }

        // Go back to home page
        xTaskNotifyGive(Task_Home_Page_Handle);
    }
}
