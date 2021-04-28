/*
 * task_setting_page.c
 *
 *  Created on: Apr 27, 2021
 *      Author: Justin Qiao
 */

#include "main.h"

TaskHandle_t Task_Setting_Page_Handle;

int game_duration_seconds = 120;        // Game duration in seconds, 60s ~ 300s per 30s
int number_of_colors = 11;       // Number of colors used in game (3 to 11, default 11)

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
            // Draw the "SETTINGS" title to LCD
            lcd_draw_image(
                    67,
                    20,
                    titleWidthPixels,
                    titleHeightPixels,
                    titleBitmaps,
                    LCD_COLOR_YELLOW,
                    LCD_COLOR_BLACK
            );

            // Set game duration in seconds
            set_game_duration_seconds();

            // Set number of colors
            set_number_of_colors();

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

/******************************************************************************
 *
 ******************************************************************************/
void set_game_duration_seconds()
{
    static int game_duration_helper = 3;        // 1 = 60s, 2 = 90s, ... 9 = 300s

    while(1)
    {
        // Print "GAME DURATION" to LCD
        lcd_print_char(20, 40, 'G');
        lcd_print_char(28, 40, 'A');
        lcd_print_char(36, 40, 'M');
        lcd_print_char(44, 40, 'E');

        lcd_print_char(57, 40, 'D');
        lcd_print_char(65, 40, 'U');
        lcd_print_char(73, 40, 'R');
        lcd_print_char(81, 40, 'A');
        lcd_print_char(89, 40, 'T');
        lcd_print_char(95, 40, 'I');
        lcd_print_char(102, 40, 'O');
        lcd_print_char(110, 40, 'N');

        // Print "[S1] CHANGE" to LCD
        lcd_print_char(20, 90, '[');
        lcd_print_char(28, 90, 'S');
        lcd_print_char(36, 90, '1');
        lcd_print_char(44, 90, ']');

        lcd_print_char(54, 90, 'C');
        lcd_print_char(62, 90, 'H');
        lcd_print_char(70, 90, 'A');
        lcd_print_char(78, 90, 'N');
        lcd_print_char(86, 90, 'G');
        lcd_print_char(94, 90, 'E');

        // Print "[S2] CONFIRM" to LCD
        lcd_print_char(20, 102, '[');
        lcd_print_char(28, 102, 'S');
        lcd_print_char(36, 102, '2');
        lcd_print_char(44, 102, ']');

        lcd_print_char(54, 102, 'C');
        lcd_print_char(62, 102, 'O');
        lcd_print_char(70, 102, 'N');
        lcd_print_char(78, 102, 'F');
        lcd_print_char(83, 102, 'I');
        lcd_print_char(89, 102, 'R');
        lcd_print_char(97, 102, 'M');

        char a, b, c;       // Used to store parsed digits in chars
        int_to_three_chars(game_duration_seconds, &a, &b, &c);        // Parse current number

        // Cover original number
        lcd_draw_rectangle(
          67,
          55,
          30,
          15,
          LCD_COLOR_BLACK
        );

        // Print current duration of game to LCD
        lcd_print_char(54, 55, a);
        lcd_print_char(62, 55, b);
        lcd_print_char(70, 55, c);

        // Print "SECONDS" under the duration number
        lcd_print_char(40, 70, 'S');
        lcd_print_char(48, 70, 'E');
        lcd_print_char(56, 70, 'C');
        lcd_print_char(64, 70, 'O');
        lcd_print_char(72, 70, 'N');
        lcd_print_char(80, 70, 'D');
        lcd_print_char(88, 70, 'S');

        // Wait until a button is pressed
        while(!S1_PRESSED && !S2_PRESSED){}

        // If S1 was pressed, update game duration
        if(S1_PRESSED)
        {
            S1_PRESSED = false;     // Reset S1

            // Update game_duration_seconds
            game_duration_helper = (game_duration_helper + 1) % 10;
            if(game_duration_helper == 0)
            {
                game_duration_helper = 1;
            }
            game_duration_seconds = game_duration_helper * 30 + 30;
        }
        else if(S2_PRESSED)     // If S2 was pressed, go to next setting
        {
            S2_PRESSED = false;     // Reset S2

            // Cover current setting
            lcd_draw_rectangle(
              67,
              60,
              100,
              50,
              LCD_COLOR_BLACK
            );

            break;      // Go to next setting
        }
    }
}

/******************************************************************************
 *
 ******************************************************************************/
void set_number_of_colors()
{
    while(1)
    {
        // Print "NUM OF COLORS" to LCD
        lcd_print_char(20, 40, 'N');
        lcd_print_char(28, 40, 'U');
        lcd_print_char(36, 40, 'M');

        lcd_print_char(49, 40, 'O');
        lcd_print_char(57, 40, 'F');

        lcd_print_char(70, 40, 'C');
        lcd_print_char(78, 40, 'O');
        lcd_print_char(86, 40, 'L');
        lcd_print_char(94, 40, 'O');
        lcd_print_char(102, 40, 'R');
        lcd_print_char(110, 40, 'S');

        // Print "[S1] CHANGE" to LCD
        lcd_print_char(20, 90, '[');
        lcd_print_char(28, 90, 'S');
        lcd_print_char(36, 90, '1');
        lcd_print_char(44, 90, ']');

        lcd_print_char(54, 90, 'C');
        lcd_print_char(62, 90, 'H');
        lcd_print_char(70, 90, 'A');
        lcd_print_char(78, 90, 'N');
        lcd_print_char(86, 90, 'G');
        lcd_print_char(94, 90, 'E');

        // Print "[S2] CONFIRM" to LCD
        lcd_print_char(20, 102, '[');
        lcd_print_char(28, 102, 'S');
        lcd_print_char(36, 102, '2');
        lcd_print_char(44, 102, ']');

        lcd_print_char(54, 102, 'C');
        lcd_print_char(62, 102, 'O');
        lcd_print_char(70, 102, 'N');
        lcd_print_char(78, 102, 'F');
        lcd_print_char(83, 102, 'I');
        lcd_print_char(89, 102, 'R');
        lcd_print_char(97, 102, 'M');

        char a, b, c;       // Used to store parsed digits in chars
        int_to_three_chars(number_of_colors, &a, &b, &c);        // Parse current number

        // Cover original number
        lcd_draw_rectangle(
          67,
          55,
          30,
          15,
          LCD_COLOR_BLACK
        );

        // Print current number of colors to LCD
        lcd_print_char(54, 55, a);
        lcd_print_char(62, 55, b);
        lcd_print_char(70, 55, c);

        // Wait until a button is pressed
        while(!S1_PRESSED && !S2_PRESSED){}

        // If S1 was pressed, update number
        if(S1_PRESSED)
        {
            S1_PRESSED = false;     // Reset S1

            // Update number
            number_of_colors = (number_of_colors + 1) % 12;

            // Make sure number is >= 3
            if(number_of_colors < 3)
            {
                number_of_colors = 3;
            }
        }
        else if(S2_PRESSED)     // If S2 was pressed, go to next setting
        {
            S2_PRESSED = false;     // Reset S2

            // Cover current setting
            lcd_draw_rectangle(
              67,
              50,
              100,
              30,
              LCD_COLOR_BLACK
            );

            break;      // Go to next setting
        }
    }
}
