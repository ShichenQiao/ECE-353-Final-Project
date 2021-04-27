/*
 * task_home_page.c
 *
 *  Created on: Apr 27, 2021
 *      Author: Justin Qiao
 */

#include <main.h>

TaskHandle_t Task_Home_Page_Handle;

/******************************************************************************
 *
 ******************************************************************************/
void Task_Home_Page(void *pvParameters)
{
    while(1)
    {
        // Print "[S1] BEGIN" to LCD
        lcd_print_char(15, 73, '[');
        lcd_print_char(23, 73, 'S');
        lcd_print_char(31, 73, '1');
        lcd_print_char(39, 73, ']');

        lcd_print_char(49, 73, 'N');
        lcd_print_char(57, 73, 'E');
        lcd_print_char(65, 73, 'W');
        lcd_print_char(73, 73, 'G');
        lcd_print_char(81, 73, 'A');
        lcd_print_char(89, 73, 'M');
        lcd_print_char(97, 73, 'E');

        // Print "[S2] HOME" to LCD
        lcd_print_char(15, 85, '[');
        lcd_print_char(23, 85, 'S');
        lcd_print_char(31, 85, '2');
        lcd_print_char(39, 85, ']');

        lcd_print_char(49, 85, 'S');
        lcd_print_char(57, 85, 'E');
        lcd_print_char(65, 85, 'T');
        lcd_print_char(73, 85, 'T');
        lcd_print_char(81, 85, 'I');
        lcd_print_char(89, 85, 'N');
        lcd_print_char(97, 85, 'G');

        while(!S1_PRESSED && !S2_PRESSED){}

        if(S1_PRESSED)
        {
            S1_PRESSED = false;     // Reset S1

            // Cover home page
            lcd_draw_rectangle(
              67,
              67,
              132,
              132,
              LCD_COLOR_BLACK
            );

            // Notify Task_Breaker to enter Gaming mode
            xTaskNotifyGive(Task_Breaker_Handle);

            // Wait until re-entry to home page
            ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        }
        else if(S2_PRESSED)
        {
            S2_PRESSED = false;     // Reset S2

            // Cover home page
            lcd_draw_rectangle(
              67,
              67,
              132,
              132,
              LCD_COLOR_BLACK
            );

            // Notify Task_Setting_Page to enter Setting mode
            xTaskNotifyGive(Task_Setting_Page_Handle);

            // Wait until re-entry to home page
            ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        }
    }
}


