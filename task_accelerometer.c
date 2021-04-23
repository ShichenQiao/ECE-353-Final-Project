/*
 * task_accelerometer.c
 *
 *  Created on: Apr 10, 2021
 *      Author: Justin Qiao
 */

#include <main.h>

TaskHandle_t Task_Accelerometer_Handle;

// These variables are restricted to this file
volatile uint32_t ACCELEROMETER_X_DIR = 0;
volatile uint32_t ACCELEROMETER_Y_DIR = 0;

/******************************************************************************
* Initialize hardware resources for the Accelerometer on MKII.
* The X reading will be stored in MEM[0] and the Y reading will be stored in MEM[1].
* The interrupt priority of the Accelerometer will be 2.
******************************************************************************/
void Task_Accelerometer_Init(void)
{
    // Configure the X direction as an analog input pin
    P6->SEL0 |= BIT1;
    P6->SEL1 |= BIT1;

    // Configure the Y direction as an analog input pin
    P4->SEL0 |= BIT0;
    P4->SEL1 |= BIT0;

    // Configure CTL0 to sample 16-times in pulsed sample mode
    // This is a sequence of samples.
    ADC14->CTL0 = ADC14_CTL0_SHT0_2 | ADC14_CTL0_SHP | ADC14_CTL0_CONSEQ_1;

    // Configure CTL1 to return 12-bit values
    ADC14->CTL1 = ADC14_CTL1_RES_2;

    // Associate the X direction analog signal with MEM[0]
    ADC14->MCTL[0] |= ADC14_MCTLN_INCH_14;

    // Associate the Y direction analog signal with MEM[1]
    // This is the end of a sequence.
    ADC14->MCTL[1] |= ADC14_MCTLN_INCH_13 | ADC14_MCTLN_EOS;

    // Enable interrupts AFTER a value is written into MEM[1]
    ADC14->IER0 |= ADC14_IER0_IE1 ;

    // Enable ADC Interrupt
    NVIC_EnableIRQ(ADC14_IRQn);

    // Interrupt priorities is set to be 2
    NVIC_SetPriority(ADC14_IRQn,2);

    // Turn ADC ON
    ADC14->CTL0 |= ADC14_CTL0_ON;

}

/******************************************************************************
* Bottom Half Task. Examines the ADC data from the Accelerometer on the MKII.
******************************************************************************/
void Task_Accelerometer_Bottom_Half(void *pvParameters)
{
    BAR_CMD_t command;     // Message prepared to be sent

    while(1)
    {
        // Wait until waken by Top Half
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        // Send proper message to Queue_Breaker according to Accelerometer readings
        if(ACCELEROMETER_X_DIR < VOLT_1P40)      // Go Left
        {
            command = BAR_CMD_LEFT;
            xQueueSend(Queue_Breaker, &command, portMAX_DELAY);
        }
        else if(ACCELEROMETER_X_DIR > VOLT_1P80)     // Go Right
        {
            command = BAR_CMD_RIGHT;
            xQueueSend(Queue_Breaker, &command, portMAX_DELAY);
        }
        else if(ACCELEROMETER_Y_DIR < VOLT_1P40)     // Go Down
        {
            command = BAR_CMD_DOWN;
            xQueueSend(Queue_Breaker, &command, portMAX_DELAY);
        }
        else if(ACCELEROMETER_Y_DIR > VOLT_1P80)     // Go Up
        {
            command = BAR_CMD_UP;
            xQueueSend(Queue_Breaker, &command, portMAX_DELAY);
        }
        else        // No movements of the tank
        {
            command = BAR_CMD_CENTER;
            // If there is no command from the console window, send BAR_CMD_CENTER
            if(RX_INDEX == 0)
                xQueueSend(Queue_Breaker, &command, portMAX_DELAY);
        }
    }
}


/******************************************************************************
* Top Half of ADC14 Handler.
******************************************************************************/
void ADC14_IRQHandler(void)
{
    BaseType_t xHigherPriorityTaskWoken;

    // Read the X value of the Accelerometer
    ACCELEROMETER_X_DIR = ADC14->MEM[0];

    // Read the Y value of the Accelerometer
    ACCELEROMETER_Y_DIR = ADC14->MEM[1];

    // Send a Task Notification to the Task_Accelerometer_Bottom_Half task
    vTaskNotifyGiveFromISR(
        Task_Accelerometer_Handle,
        &xHigherPriorityTaskWoken
    );

    // Context switch, if needed
    portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}



