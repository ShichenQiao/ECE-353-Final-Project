/*
 * task_home_page.c
 *
 *  Created on: Apr 27, 2021
 *      Author: Justin Qiao
 */

#include <main.h>

TaskHandle_t Task_Home_Page_Handle;

/******************************************************************************
 * Home page of the game BREAKER. Plays theme song with LED flashing.
 * Press S1 to go to Gaming mode.
 * Press S2 to go to Setting mode.
 ******************************************************************************/
void Task_Home_Page(void *pvParameters)
{
    // Never return
    while(1)
    {
        xSemaphoreTake(Sem_Console, portMAX_DELAY);

        // Print home page instructions
        printf("\n\r");
        printf("*** You are at the home page ***\n\r");
        printf("\n\r");
        printf("* Press S1 to start a new Game *\n\r");
        printf("* Press S2 to go to settings *\n\r");
        printf("\n\r");

        xSemaphoreGive(Sem_Console);

        // Cover home page
        lcd_draw_rectangle(
          67,
          67,
          132,
          132,
          background_color
        );

        // Draw logo to LCD
        lcd_draw_image(
                67,
                20,
                logoWidthPixels,
                logoHeightPixels,
                logoBitmaps,
                LCD_COLOR_YELLOW,
                background_color
        );

        // Print "[S1] NEW GAME" to LCD
        lcd_print_char(20, 50, '[');
        lcd_print_char(28, 50, 'S');
        lcd_print_char(36, 50, '1');
        lcd_print_char(44, 50, ']');

        lcd_print_char(51, 50, 'N');
        lcd_print_char(58, 50, 'E');
        lcd_print_char(68, 50, 'W');

        lcd_print_char(82, 50, 'G');
        lcd_print_char(90, 50, 'A');
        lcd_print_char(98, 50, 'M');
        lcd_print_char(106, 50, 'E');

        // Print "[S2] SETTINGS" to LCD
        lcd_print_char(20, 62, '[');
        lcd_print_char(28, 62, 'S');
        lcd_print_char(36, 62, '2');
        lcd_print_char(44, 62, ']');

        lcd_print_char(51, 62, 'S');
        lcd_print_char(59, 62, 'E');
        lcd_print_char(67, 62, 'T');
        lcd_print_char(74, 62, 'T');
        lcd_print_char(79, 62, 'I');
        lcd_print_char(86, 62, 'N');
        lcd_print_char(95, 62, 'G');
        lcd_print_char(104, 62, 'S');

        // Draw background
        lcd_draw_image(
                55,
                100,
                tankWidthPixels,
                tankHeightPixels,
                tank_leftBitmaps,
                LCD_COLOR_BLUE,
                background_color
        );
        lcd_draw_image(
                20,
                100,
                ballWidthPixels,
                ballHeightPixels,
                image_ball,
                LCD_COLOR_BLUE,
                background_color
        );
        lcd_draw_image(
                20,
                80,
                tankWidthPixels,
                tankHeightPixels,
                tank_rightBitmaps,
                LCD_COLOR_GREEN,
                background_color
        );
        lcd_draw_image(
                65,
                80,
                ballWidthPixels,
                ballHeightPixels,
                image_ball,
                LCD_COLOR_GREEN,
                background_color
        );
        lcd_draw_image(
                85,
                110,
                tankWidthPixels,
                tankHeightPixels,
                tank_upBitmaps,
                LCD_COLOR_RED,
                background_color
        );
        lcd_draw_image(
                110,
                90,
                tankWidthPixels,
                tankHeightPixels,
                tank_downBitmaps,
                LCD_COLOR_CYAN,
                background_color
        );

        // Play theme song until S1 or S2 is pressed
        while(music_play_song_shine()){}

        // If S1 was pressed, go to Gaming mode
        if(S1_PRESSED)
        {
            S1_PRESSED = false;     // Reset S1

            // Cover home page
            lcd_draw_rectangle(
              67,
              67,
              132,
              132,
              background_color
            );

            // Notify Task_Breaker to enter Gaming mode
            xTaskNotifyGive(Task_Breaker_Handle);

            // Wait until re-entry to home page
            ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        }
        else if(S2_PRESSED)     // If S2 was pressed, go to Setting mode
        {
            S2_PRESSED = false;     // Reset S2

            // Cover home page
            lcd_draw_rectangle(
              67,
              67,
              132,
              132,
              background_color
            );

            // Notify Task_Setting_Page to enter Setting mode
            xTaskNotifyGive(Task_Setting_Page_Handle);

            // Wait until re-entry to home page
            ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        }
    }
}


