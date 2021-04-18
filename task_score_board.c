/*
 * task_score_board.c
 *
 *  Created on: Apr 14, 2021
 *      Author: Justin Qiao
 */

#include <main.h>

TaskHandle_t Task_Score_Board_Handle;

int time_left = 0;      // The time left in an ongoing game

/******************************************************************************
 * This task manages the score board in Gaming mode
 ******************************************************************************/
void Task_Score_Board(void *pvParameters)
{
    char a, b, c;

    lcd_print_char(15, 10, 'H');
    lcd_print_char(20, 10, 'I');
    lcd_print_char(24, 10, 'T');
    lcd_print_char(30, 10, ':');

    lcd_print_char(70, 10, 'T');
    lcd_print_char(74, 10, 'I');
    lcd_print_char(80, 10, 'M');
    lcd_print_char(87, 10, 'E');
    lcd_print_char(94, 10, ':');

    update_score();

    int_to_three_chars(time_left, &a, &b, &c);

    xSemaphoreTake(Sem_LCD, portMAX_DELAY);

    lcd_draw_rectangle(
      110,
      10,
      30,
      10,
      LCD_COLOR_BLACK
    );

    xSemaphoreGive(Sem_LCD);

    lcd_print_char(101, 10, a);
    lcd_print_char(108, 10, b);
    lcd_print_char(115, 10, c);

    while(1)
    {
        // Wait until a game is started to manage the score board
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        while (time_left >= 0)
        {
            int_to_three_chars(time_left, &a, &b, &c);

            xSemaphoreTake(Sem_LCD, portMAX_DELAY);

            lcd_draw_rectangle(
              110,
              10,
              30,
              10,
              LCD_COLOR_BLACK
            );

            xSemaphoreGive(Sem_LCD);

            lcd_print_char(101, 10, a);
            lcd_print_char(108, 10, b);
            lcd_print_char(115, 10, c);

            time_left--;
            vTaskDelay(pdMS_TO_TICKS(1000));
        }

        while(1)
        {
            // Freeze LCD
            xSemaphoreTake(Sem_LCD, portMAX_DELAY);
        }
    }
}

/******************************************************************************
 * Helper method to print char c in Arial on the LCD screen, centered at (x, y)
 ******************************************************************************/
void lcd_print_char(uint16_t x, uint16_t y, char c)
{
    uint16_t width = get_width(c);
    uint16_t height = get_height(c);
    uint16_t offset = get_offset(c);

    xSemaphoreTake(Sem_LCD, portMAX_DELAY);

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
 *
 ******************************************************************************/
void int_to_three_chars(int num, char *a, char *b, char *c)
{
    if(num >= 999)
    {
        *a = '9';
        *b = '9';
        *c = '9';
        return;
    }
    int hundreds, tens, ones;
    hundreds = num / 100;
    tens = (num / 10) % 10;
    ones = num % 10;

    *a = hundreds + '0';
    *b = tens + '0';
    *c = ones + '0';
}

void update_score(){
    char a, b, c;

    int_to_three_chars(score, &a, &b, &c);

    xSemaphoreTake(Sem_LCD, portMAX_DELAY);

    lcd_draw_rectangle(
      50,
      10,
      30,
      10,
      LCD_COLOR_BLACK
    );

    xSemaphoreGive(Sem_LCD);

    lcd_print_char(37, 10, a);
    lcd_print_char(44, 10, b);
    lcd_print_char(51, 10, c);
}
