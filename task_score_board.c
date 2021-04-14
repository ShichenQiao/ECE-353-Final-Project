/*
 * task_score_board.c
 *
 *  Created on: Apr 14, 2021
 *      Author: Justin Qiao
 */

#include <main.h>

TaskHandle_t Task_Score_Board_Handle;

/******************************************************************************
 * This task manages the score board in Gaming mode
 ******************************************************************************/
void Task_Score_Board(void *pvParameters)
{
    while(1)
    {
        // Wait until a game is started to manage the score board
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        lcd_print_char(15, 10, 'H');
        lcd_print_char(20, 10, 'I');
        lcd_print_char(24, 10, 'T');
        lcd_print_char(30, 10, ':');

        lcd_print_char(37, 10, '0');
        lcd_print_char(44, 10, '0');
        lcd_print_char(51, 10, '0');

        lcd_print_char(70, 10, 'M');
        lcd_print_char(75, 10, 'I');
        lcd_print_char(80, 10, 'S');
        lcd_print_char(87, 10, 'S');
        lcd_print_char(94, 10, ':');

        lcd_print_char(101, 10, '0');
        lcd_print_char(108, 10, '0');
        lcd_print_char(115, 10, '0');

        vTaskDelay(pdMS_TO_TICKS(20));
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
