/*
 * main.c
 *
 *  Created on: Apr 10, 2021
 *      Author: Justin Qiao
 */

#include "main.h"

/*
 *  ======== main ========
 */
int main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    // Hardware initializations
    Task_Breaker_Init();
    Task_Accelerometer_Init();
    ece353_MKII_S1_Init();
    ece353_MKII_S2_Init();
    ece353_rgb_init();
    MKII_RGB_Init();
    Task_Console_Init();
    music_init();
    i2c_init();
    opt3001_init();;

    // Enable interrupts
    __enable_irq();

    // Print welcome message
    printf("\n\r");
    printf("*********************************************\n\r");
    printf("* Welcome to BREAKER!\n\r");
    printf("*********************************************\n\r");
    printf("\n\r");

    // Main Gaming mode task
    xTaskCreate
     (   Task_Breaker,
         "Task_Breaker",
         configMINIMAL_STACK_SIZE,
         NULL,
         1,
         &Task_Breaker_Handle
     );

    // Task that manage enemy square generations and breaks
    xTaskCreate
     (   Task_Enemy,
         "Task_Enemy",
         configMINIMAL_STACK_SIZE,
         NULL,
         1,
         &Task_Enemy_Handle
     );

    // Task that manage the score board
    xTaskCreate
     (   Task_Score_Board,
         "Task_Score_Board",
         configMINIMAL_STACK_SIZE,
         NULL,
         1,
         &Task_Score_Board_Handle
     );

    // Shared timer around all tasks
    xTaskCreate
    (   Task_Timer,
        "Task_Timer",
        configMINIMAL_STACK_SIZE,
        NULL,
        2,
        &Task_Timer_Handle
    );

    // Task to get data from ADC and pass commands to other tasks via queue
    xTaskCreate
    (   Task_Accelerometer_Bottom_Half,
        "Task_Accelerometer_Bottom_Half",
        configMINIMAL_STACK_SIZE,
        NULL,
        3,
        &Task_Accelerometer_Handle
    );

    // Task to get commands from console window
    xTaskCreate
     (   Task_Console_Bottom_Half,
         "Task_Console_Bottom_Half",
         configMINIMAL_STACK_SIZE,
         NULL,
         2,
         &Task_Console_Handle
     );

    /* Start the FreeRTOS scheduler */
    vTaskStartScheduler();

    while(1){};     // Never return
    return 0;
}

//*****************************************************************************
//
//! \brief Application defined malloc failed hook
//!
//! \param  none
//!
//! \return none
//!
//*****************************************************************************
void vApplicationMallocFailedHook()
{
    /* Handle Memory Allocation Errors */
    while(1)
    {
    }
}

//*****************************************************************************
//
//! \brief Application defined stack overflow hook
//!
//! \param  none
//!
//! \return none
//!
//*****************************************************************************
void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName)
{
    //Handle FreeRTOS Stack Overflow
    while(1)
    {
    }
}
