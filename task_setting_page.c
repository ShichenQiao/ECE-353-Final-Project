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
            lcd_print_char(51, 62, 'S');
            lcd_print_char(59, 62, 'E');
            lcd_print_char(67, 62, 'T');
            lcd_print_char(74, 62, 'T');
            lcd_print_char(79, 62, 'I');
            lcd_print_char(86, 62, 'N');
            lcd_print_char(95, 62, 'G');
            lcd_print_char(104, 62, 'S');

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
