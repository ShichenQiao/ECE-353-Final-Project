/*
 * task_score_board.c
 *
 *  Created on: Apr 14, 2021
 *      Author: Justin Qiao
 */

#include <main.h>

TaskHandle_t Task_Score_Board_Handle;

/******************************************************************************
 * This function resets the score board. It will be called in reset_game().
 ******************************************************************************/
void score_board_reset()
{
    char a, b, c;       // Used to store parsed digits in chars

    // Print "HIT:" on the left side of the score board on LCD
    lcd_print_char(15, 10, 'H');
    lcd_print_char(20, 10, 'I');
    lcd_print_char(24, 10, 'T');
    lcd_print_char(30, 10, ':');

    // Print "TIME:" on the right side of the score board on LCD
    lcd_print_char(70, 10, 'T');
    lcd_print_char(74, 10, 'I');
    lcd_print_char(80, 10, 'M');
    lcd_print_char(87, 10, 'E');
    lcd_print_char(94, 10, ':');

    // Print score to LCD, should be 000
    update_score();

    // Parse time_left in to a, b, and c
    int_to_three_chars(time_left, &a, &b, &c);

    xSemaphoreTake(Sem_LCD, portMAX_DELAY);

    // Cover original time value
    lcd_draw_rectangle(
      110,
      10,
      30,
      10,
      LCD_COLOR_BLACK
    );

    xSemaphoreGive(Sem_LCD);

    // Print time_left to LCD, should be the length of the new game
    lcd_print_char(101, 10, a);
    lcd_print_char(108, 10, b);
    lcd_print_char(115, 10, c);
}

/******************************************************************************
 * This task manages the score board in Gaming mode. This function will count
 * down the time left in an ongoing game, and terminates the game when time is
 * up. The score section of the score board will be managed by update_score(),
 * which updates the score shown on LCD only when a color-matched collision
 * between the ball and a square occurs.
 ******************************************************************************/
void Task_Score_Board(void *pvParameters)
{
    char a, b, c;       // Used to store parsed digits in chars

    while(1)
    {
        // Wait until a game is started to manage the score board
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        // Count down time_left until 0
        while (time_left >= 0)
        {
            // Parse time_left in to a, b, and c
            int_to_three_chars(time_left, &a, &b, &c);

            xSemaphoreTake(Sem_LCD, portMAX_DELAY);

            // Cover original time value
            lcd_draw_rectangle(
              110,
              10,
              30,
              10,
              LCD_COLOR_BLACK
            );

            xSemaphoreGive(Sem_LCD);

            // Print time_left to LCD
            lcd_print_char(101, 10, a);
            lcd_print_char(108, 10, b);
            lcd_print_char(115, 10, c);

            // Count down by 1 second
            time_left--;

            // Count 1 second
            vTaskDelay(pdMS_TO_TICKS(1000));
        }

        // If the counter reaches 0, terminate the game
        game_on_going = false;
    }
}

/******************************************************************************
 * Helper method to print char c in Arial on the LCD screen, centered at (x, y).
 ******************************************************************************/
void lcd_print_char(uint16_t x, uint16_t y, char c)
{
    // Get information about c from font_arial
    uint16_t width = get_width(c);
    uint16_t height = get_height(c);
    uint16_t offset = get_offset(c);

    xSemaphoreTake(Sem_LCD, portMAX_DELAY);

    // Print c to specified position on LCD
    lcd_draw_image(
            x,
            y,
            width,
            height,
            arial_8ptBitmaps + offset,
            LCD_COLOR_YELLOW,
            LCD_COLOR_BLACK
    );

    xSemaphoreGive(Sem_LCD);
}

/******************************************************************************
 * Helper method to parse an int num into three digits as chars. The hundreds
 * place will be stored in *a, the tens place will be stored in *b, and the
 * ones place will be stored in *c.
 * If a place does not exist in num, set its corresponding char to '0'.
 * If num is greater than 999, set all of a, b, and c to '9'.
 ******************************************************************************/
void int_to_three_chars(int num, char *a, char *b, char *c)
{
    // If num is greater than 999, set all of a, b, and c to '9' and return
    if(num >= 999)
    {
        *a = '9';
        *b = '9';
        *c = '9';
        return;
    }

    // Otherwise parse the given num into digits
    int hundreds, tens, ones;
    hundreds = num / 100;
    tens = (num / 10) % 10;
    ones = num % 10;

    // Store digits as chars pointed by the given pointers a, b, and c
    *a = hundreds + '0';
    *b = tens + '0';
    *c = ones + '0';
}

/******************************************************************************
 * This function manages the score section on the score board. It will be called
 * from check_squares() as defined in task_enemy when a color-matched collision
 * between the ball and a square occurs. This function will update the score
 * shown on LCD accordingly when called.
 ******************************************************************************/
void update_score(){
    char a, b, c;       // Used to store parsed digits in chars

    // Parse the current score
    int_to_three_chars(score, &a, &b, &c);

    xSemaphoreTake(Sem_LCD, portMAX_DELAY);

    // Cover original score value
    lcd_draw_rectangle(
      50,
      10,
      30,
      10,
      LCD_COLOR_BLACK
    );

    xSemaphoreGive(Sem_LCD);

    // Print score to LCD
    lcd_print_char(37, 10, a);
    lcd_print_char(44, 10, b);
    lcd_print_char(51, 10, c);
}

