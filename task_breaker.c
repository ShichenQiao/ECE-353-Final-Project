/*
 * task_breaker.c
 *
 *  Created on: Apr 10, 2021
 *      Author: Justin Qiao
 */

#include <main.h>

#define QUEUE_LEN  10

TaskHandle_t Task_Breaker_Handle;
QueueHandle_t Queue_Breaker;
SemaphoreHandle_t Sem_LCD;

// Shared information about the ball
uint8_t ball_x;
uint8_t ball_y;
bool ball_launched;      // Flag variable, true if the ball has been launched
int ball_dir;   // 0 for LEFT, 1 for RIGHT, 2 for UP, and 3 for DOWN

// Shared global variable that stores the lux number read from opt3001
float lux = 0.0;

// Back ground color black, true means background color is black false is white.
bool bgc_black = true;

// Shared coordinates of the tank
uint8_t tank_x;
uint8_t tank_y;
Color_t tank_color;
uint16_t tank_lcd_color;
int tank_dir;   // 0 for LEFT, 1 for RIGHT, 2 for UP, and 3 for DOWN

bool occupied[LCD_VERTICAL_MAX][LCD_HORIZONTAL_MAX];    // Matrix to record whether a pixel on the LCD is occupied

int score;      // Keeps track of the score of a game
int time_left;      // The time left in an ongoing game
bool game_on_going = false;        // True if a game is ongoing, false otherwise

/******************************************************************************
 * Initialize the 3 GPIO pins that control the RGB LED on the MKII.
 ******************************************************************************/
void MKII_RGB_Init()
{
    // RED      : P2.6
    // GREEN    : P2.4
    // BLUE     : P5.6
    P2->DIR |= BIT6;
    P2->DIR |= BIT4;
    P5->DIR |= BIT6;
}

/******************************************************************************
 * This function will initialize Queue_Breaker, the LCD and Sem_LCD.
 ******************************************************************************/
void Task_Breaker_Init(void)
{
    // Initialize Queue_Breaker
    Queue_Breaker = xQueueCreate(QUEUE_LEN, sizeof(TANK_CMD_t));

    // Initialize LCD
    Crystalfontz128x128_Init();

    // Initialize Sem_LCD
    Sem_LCD = xSemaphoreCreateBinary();
    xSemaphoreGive(Sem_LCD);
}

/******************************************************************************
 * This function resets a Breaker game. All game parameters will be set to
 * default, and the LCD will be redraw as its initial condition.
 ******************************************************************************/
void reset_game()
{
    xSemaphoreTake(Sem_LCD, portMAX_DELAY);

    // Clear LCD
    lcd_draw_rectangle(
      LCD_HORIZONTAL_MAX / 2,
      LCD_VERTICAL_MAX / 2,
      LCD_HORIZONTAL_MAX,
      LCD_VERTICAL_MAX,
      LCD_COLOR_BLACK
    );

    xSemaphoreGive(Sem_LCD);

    // Initialize position of the ball
    ball_x = 67;
    ball_y = 124;

    // Ball is not launched and facing Up by default
    ball_launched = false;
    ball_dir = 2;

    // Initialize position and color of the tank
    tank_x = 67;
    tank_y = 123;
    tank_color = GREEN;
    tank_lcd_color = get_lcd_color(tank_color);

    // Tank is facing Up by default
    tank_dir = 2;

    int i, j;   // Loop variables

    // Setup occupied matrix
    for(i = 0; i < LCD_VERTICAL_MAX; i++)
    {
        for(j = 0; j < LCD_HORIZONTAL_MAX; j++)
        {
            // Any pixel outside the battle field boarders are considered occupied
            if(i < 25 || i > 129 || j < 7 || j > 124)
            {
                occupied[i][j] = true;
            }
            else
            {
                occupied[i][j] = false;
            }
        }
    }

    xSemaphoreTake(Sem_LCD, portMAX_DELAY);

    // Draw initial image of the tank
    lcd_draw_image(
            tank_x,
            tank_y,
            tankWidthPixels,
            tankHeightPixels,
            tank_upBitmaps,
            tank_lcd_color,
            LCD_COLOR_BLACK
    );

    // Draw boarders on LCD
    lcd_draw_rectangle(4, 67, 5, 128, LCD_COLOR_RED);       // Left boarder
    lcd_draw_rectangle(127, 67, 5, 128, LCD_COLOR_RED);       // Right boarder
    lcd_draw_rectangle(67, 22, 128, 5, LCD_COLOR_RED);       // Upper boarder

    xSemaphoreGive(Sem_LCD);

    // Set game duration, default game length is 120 seconds
    time_left = game_duration_seconds;

    // Reset score
    score = 0;

    // Reset score board
    score_board_reset();

    // Reset enemies
    enemy_reset();
}

/******************************************************************************
 * This is the main task controlling the gaming mode. Commands can be sent to
 * Queue_Breaker to be processed.
 ******************************************************************************/
void Task_Breaker(void *pvParameters)
{
    // Helper variables to recored previous position of the ball right before LCD updates
    uint8_t ball_prev_x;
    uint8_t ball_prev_y;

    int i;   // Loop variables

    // Temp variables, store draw frame information of different images
    // (x0, y0) is the left upper corner of an image
    // (x1, y1) is the right lower corner of an image
    int x0, x1, y0, y1;

    while(1)
    {
        // Wait until entering game mode from the home page
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        while(1)
        {
            // Reset the game
            reset_game();

            // Print pre-game message
            print_pre_game_message();

            // Play theme song until S1 or S2 is pressed
            while(music_play_song_shine()){}

            S1_PRESSED = false;     // Reset S1

            // If S2 pressed, exit Gaming mode
            if(S2_PRESSED)
            {
                S2_PRESSED = false;
                break;
            }
            // Otherwise, loop back and game will be reset

            // Cover pre-game message
            lcd_draw_rectangle(
              67,
              67,
              84,
              54,
              LCD_COLOR_BLACK
            );

            // Set flag showing a game is on going
            game_on_going = true;

            // Notify Task_Score_Board to initialize with the given game length
            xTaskNotifyGive(Task_Score_Board_Handle);

            // Indicate whether the shot ball music has been played.
            bool shot_music = false;

            // Enter the Gaming Mode
            while(game_on_going)
            {
                TANK_CMD_t message;     // Message received from Queue_Breaker

                bool tank_update = true;     // Reset flag

                // Notify Task_Enemy to start generating enemy squares
                xTaskNotifyGive(Task_Enemy_Handle);

                // Wait indefinitely until a message is received
                xQueueReceive(Queue_Breaker, &message, portMAX_DELAY);

                // Get the current draw frame of the tank
                lcd_get_draw_frame(tank_x, tank_y, tankWidthPixels, tankHeightPixels, &x0, &x1, &y0, &y1);

                // Set background color
                set_bgc();

                // Examine the message received, move the tank accordingly without passing boarders or running into enemies
                switch(message){
                    case TANK_CMD_LEFT:
                    {
                        // Check Left boarder of the tank
                        for(i = y0; i < y1; i++)
                        {
                            if(occupied[i][x0 - 1])
                            {
                                tank_update = false;
                            }
                        }

                        // Update direction to Left
                        tank_dir = 0;

                        // If the tank is allowed to pass, go Left by 1 pixel
                        if(tank_update)
                        {
                            tank_x--;
                        }

                        // Update the tank
                        xSemaphoreTake(Sem_LCD, portMAX_DELAY);

                        lcd_draw_image(
                                tank_x,
                                tank_y,
                                tankWidthPixels,
                                tankHeightPixels,
                                tank_leftBitmaps,
                                tank_lcd_color,
                                LCD_COLOR_BLACK
                        );

                        xSemaphoreGive(Sem_LCD);

                        break;
                    }
                    case TANK_CMD_RIGHT:
                    {
                        // Check Right boarder of the tank
                        for(i = y0; i < y1; i++)
                        {
                            if(occupied[i][x1 + 1])
                            {
                                tank_update = false;
                            }
                        }

                        // Update direction to Right
                        tank_dir = 1;

                        // If the tank is allowed to pass, go Right by 1 pixel
                        if(tank_update)
                        {
                            tank_x++;
                        }

                        // Update the tank
                        xSemaphoreTake(Sem_LCD, portMAX_DELAY);

                        lcd_draw_image(
                                tank_x,
                                tank_y,
                                tankWidthPixels,
                                tankHeightPixels,
                                tank_rightBitmaps,
                                tank_lcd_color,
                                LCD_COLOR_BLACK
                        );

                        xSemaphoreGive(Sem_LCD);

                        break;
                    }
                    case TANK_CMD_DOWN:
                    {
                        // Check Down boarder of the tank
                        for(i = x0; i < x1; i++)
                        {
                            if(occupied[y1 + 1][i])
                            {
                                tank_update = false;
                            }
                        }

                        // Update direction to Down
                        tank_dir = 3;

                        // If the tank is allowed to pass, go Down by 1 pixel
                        if(tank_update)
                        {
                            tank_y++;
                        }

                        // Update the tank
                        xSemaphoreTake(Sem_LCD, portMAX_DELAY);

                        lcd_draw_image(
                                tank_x,
                                tank_y,
                                tankWidthPixels,
                                tankHeightPixels,
                                tank_downBitmaps,
                                tank_lcd_color,
                                LCD_COLOR_BLACK
                        );

                        xSemaphoreGive(Sem_LCD);

                        break;
                    }
                    case TANK_CMD_UP:
                    {
                        // Check Up boarder of the tank
                        for(i = x0; i < x1; i++)
                        {
                            if(occupied[y0 - 1][i])
                            {
                                tank_update = false;
                            }
                        }

                        // Update direction to Up
                        tank_dir = 2;

                        // If the tank is allowed to pass, go Up by 1 pixel
                        if(tank_update)
                        {
                            tank_y--;
                        }

                        // Update the tank
                        xSemaphoreTake(Sem_LCD, portMAX_DELAY);

                        lcd_draw_image(
                                tank_x,
                                tank_y,
                                tankWidthPixels,
                                tankHeightPixels,
                                tank_upBitmaps,
                                tank_lcd_color,
                                LCD_COLOR_BLACK
                        );

                        xSemaphoreGive(Sem_LCD);

                        break;
                    }
                    case TANK_CMD_CENTER:        // Do not update the tank in this case
                    {
                        tank_update = false;
                        break;
                    }
                    case TANK_CMD_LAUNCH:       // Launch the ball if it has not been launched
                    {
                        if(!ball_launched)
                        {
                            ball_dir = tank_dir;
                            ball_launched = true;
                        }
                        break;
                    }
                }

                // Launch the ball if haven't yet when S1 is pressed during a game
                if(!ball_launched && S1_PRESSED)
                {
                    ball_dir = tank_dir;
                    ball_launched = true;
                }

                if(!ball_launched)   // If the ball was not launched
                {
                    // Make the ball follow the tank's position
                    if(tank_update)
                    {
                        ball_reset();
                    }

                    // If not launched, stop playing music
                    shot_music = false;
                }
                else        // If the ball is launched
                {
                    // Recored the previous position of the ball for clearing before move
                    ball_prev_x = ball_x;
                    ball_prev_y = ball_y;

                    if (!shot_music) {  // If not played the music before, play the shot music
                        // Play hit sound effect
                        music_play_shot();
                        shot_music = true;
                    }

                    // If the ball is still in the range of the boarders, move it
                    if(ball_y > 32 && ball_y < 125 && ball_x > 13 && ball_x < 119)
                    {
                        // Attempt to move the ball twice as fast as the tank does
                        switch(ball_dir)
                        {
                            case 0:     // Go Left
                            {
                                ball_x -= 2;
                                break;
                            }
                            case 1:     // Go Right
                            {
                                ball_x += 2;
                                break;
                            }
                            case 2:     // Go Up
                            {
                                ball_y -= 2;
                                break;
                            }
                            case 3:     // Go Down
                            {
                                ball_y += 2;
                                break;
                            }
                        }

                        xSemaphoreTake(Sem_LCD, portMAX_DELAY);

                        // Clear previous ball's position
                        lcd_draw_image(
                                ball_prev_x,
                                ball_prev_y,
                                ballWidthPixels,
                                ballHeightPixels,
                                image_ball,
                                LCD_COLOR_BLACK,
                                LCD_COLOR_BLACK
                        );
                        // Re-draw the ball at its new position
                        lcd_draw_image(
                                ball_x,
                                ball_y,
                                ballWidthPixels,
                                ballHeightPixels,
                                image_ball,
                                tank_lcd_color,
                                LCD_COLOR_BLACK
                        );

                        xSemaphoreGive(Sem_LCD);

                        // Redraw the tank at its current position so that the ball will not cover the tank
                        tank_recover();
                    }
                    else    // If the ball is moving out of the boarder, reset it
                    {
                        ball_reset();
                    }
                }

                // Reset S1 on MKII
                S1_PRESSED = false;

                // Delay for 20mS
                vTaskDelay(pdMS_TO_TICKS(20));
            }

            // When a game ends, display end game message with score earned
            print_end_game_message();

            // Make sure excess presses at the end of the game do not influence showing the result
            S1_PRESSED = false;

            // Wait until S1 or S2 is pressed
            while(!S1_PRESSED && !S2_PRESSED){};

            // If S2 pressed, exit Gaming mode
            if(S2_PRESSED)
            {
                S2_PRESSED = false;
                break;
            }
            // Otherwise, loop back and game will be reset

            // Reset S1
            S1_PRESSED = false;

        }

        // Cover Gaming page
        lcd_draw_rectangle(
          67,
          67,
          132,
          132,
          LCD_COLOR_BLACK
        );

        // Go back to home page
        xTaskNotifyGive(Task_Home_Page_Handle);
    }
}

/******************************************************************************
 * Helper method to reset the ball according to the current position of the
 * tank, the ball will have the same center as the tank, respectively.
 ******************************************************************************/
void ball_reset(void)
{
    xSemaphoreTake(Sem_LCD, portMAX_DELAY);

    ball_launched = false;       // Reset the flag

    // Clear the previous image of the ball
    lcd_draw_image(
            ball_x,
            ball_y,
            ballWidthPixels,
            ballHeightPixels,
            image_ball,
            LCD_COLOR_BLACK,
            LCD_COLOR_BLACK
    );

    xSemaphoreGive(Sem_LCD);

    // Re-draw the tank to make sure it's not covered
    tank_recover();

    // Update the position of the ball according to the tank
    ball_x = tank_x;
    ball_y = tank_y + 1;
}

/******************************************************************************
 * Helper method to redraw the tank at its current position so that the ball
 * will not cover the tank.
 ******************************************************************************/
void tank_recover()
{
    xSemaphoreTake(Sem_LCD, portMAX_DELAY);

    // Redraw the tank at its current position
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
}

/******************************************************************************
 * Helper method to print a pre-game message with instructions.
 ******************************************************************************/
void print_pre_game_message()
{
    // Draw a box on the middle of the LCD to show pre game message
    lcd_draw_rectangle(
      67,
      67,
      84,
      54,
      LCD_COLOR_YELLOW
    );
    lcd_draw_rectangle(
      67,
      67,
      80,
      50,
      LCD_COLOR_BLACK
    );

    // Print "WELCOME" to the top row of the box
    lcd_print_char(42, 50, 'W');
    lcd_print_char(52, 50, 'E');
    lcd_print_char(60, 50, 'L');
    lcd_print_char(68, 50, 'C');
    lcd_print_char(76, 50, 'O');
    lcd_print_char(84, 50, 'M');
    lcd_print_char(92, 50, 'E');

    // Print "[S1] BEGIN" to LCD
    lcd_print_char(35, 73, '[');
    lcd_print_char(43, 73, 'S');
    lcd_print_char(51, 73, '1');
    lcd_print_char(59, 73, ']');

    lcd_print_char(69, 73, 'B');
    lcd_print_char(77, 73, 'E');
    lcd_print_char(85, 73, 'G');
    lcd_print_char(93, 73, 'I');
    lcd_print_char(100, 73, 'N');

    // Print "[S2] HOME" to LCD
    lcd_print_char(35, 85, '[');
    lcd_print_char(43, 85, 'S');
    lcd_print_char(51, 85, '2');
    lcd_print_char(59, 85, ']');

    lcd_print_char(69, 85, 'H');
    lcd_print_char(77, 85, 'O');
    lcd_print_char(85, 85, 'M');
    lcd_print_char(93, 85, 'E');
}

/******************************************************************************
 * Helper method to print a message saying game over and show the score earned.
 * Instruction for how to proceed will also be printed after a 5 second pause
 * with count down on LCD.
 ******************************************************************************/
void print_end_game_message()
{
    // Draw a box on the middle of the LCD to show end game message
    lcd_draw_rectangle(
      67,
      67,
      84,
      54,
      LCD_COLOR_YELLOW
    );
    lcd_draw_rectangle(
      67,
      67,
      80,
      50,
      LCD_COLOR_BLACK
    );

    // Print "GAME OVER!" to the top row of the box
    lcd_print_char(35, 50, 'G');
    lcd_print_char(43, 50, 'A');
    lcd_print_char(51, 50, 'M');
    lcd_print_char(59, 50, 'E');

    lcd_print_char(69, 50, 'O');
    lcd_print_char(77, 50, 'V');
    lcd_print_char(85, 50, 'E');
    lcd_print_char(93, 50, 'R');
    lcd_print_char(100, 50, '!');

    // Print "SCORE:" right below the "GAME OVER!"
    lcd_print_char(35, 60, 'S');
    lcd_print_char(43, 60, 'C');
    lcd_print_char(51, 60, 'O');
    lcd_print_char(59, 60, 'R');
    lcd_print_char(67, 60, 'E');
    lcd_print_char(72, 60, ':');

    char a, b, c;       // Used to store parsed digits in chars

    // Parse the current score
    int_to_three_chars(score, &a, &b, &c);

    // Print score to the right of "SCORE:"
    lcd_print_char(80, 60, a);
    lcd_print_char(87, 60, b);
    lcd_print_char(94, 60, c);

    // Pause 5 seconds with a counter down shown on LCD
    int pause = 5;
    while(pause > 0)
    {
        // Print "PAUSE:" at the bottom row of the box
        lcd_print_char(40, 85, 'P');
        lcd_print_char(48, 85, 'A');
        lcd_print_char(56, 85, 'U');
        lcd_print_char(64, 85, 'S');
        lcd_print_char(72, 85, 'E');
        lcd_print_char(77, 85, ':');

        // Cover original count
        lcd_draw_rectangle(
          90,
          85,
          10,
          10,
          LCD_COLOR_BLACK
        );

        // Print the count down
        lcd_print_char(90, 85, pause + '0');

        // Count down by 1 second
        pause--;
        vTaskDelay(pdMS_TO_TICKS(1000));
    }

    // Cover pause count down section with background color
    lcd_draw_rectangle(
      67,
      85,
      80,
      10,
      LCD_COLOR_BLACK
    );

    // Print "[S1] RESET" to LCD
    lcd_print_char(35, 73, '[');
    lcd_print_char(43, 73, 'S');
    lcd_print_char(51, 73, '1');
    lcd_print_char(59, 73, ']');

    lcd_print_char(69, 73, 'R');
    lcd_print_char(77, 73, 'E');
    lcd_print_char(85, 73, 'S');
    lcd_print_char(93, 73, 'E');
    lcd_print_char(100, 73, 'T');

    // Print "[S2] HOME" to LCD
    lcd_print_char(35, 85, '[');
    lcd_print_char(43, 85, 'S');
    lcd_print_char(51, 85, '2');
    lcd_print_char(59, 85, ']');

    lcd_print_char(69, 85, 'H');
    lcd_print_char(77, 85, 'O');
    lcd_print_char(85, 85, 'M');
    lcd_print_char(93, 85, 'E');

}
