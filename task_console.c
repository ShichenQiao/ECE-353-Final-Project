/*
 * task_console.c
 *
 *  Created on: Apr 22, 2021
 *      Author: Justin Qiao
 */

#include <main.h>

TaskHandle_t Task_Console_Handle;
SemaphoreHandle_t Sem_Console;

#define RX_ARRAY_SIZE 10

// Global variables used to store incoming data from RXBUF.
volatile char RX_ARRAY[RX_ARRAY_SIZE];
volatile uint16_t RX_INDEX = 0;

/******************************************************************************
 * This function configures the eUSCI_A0 to be a UART that communicates at
 * 115200 8N1.
 * The priority of the eUSCI is 2.
 ******************************************************************************/
static void console_hw_init(void)
{
    // Configure the IO pins used for the UART
    // Set 2-UART pin as secondary function
    P1->SEL0 |= BIT2 | BIT3;
    P1->SEL1 &= ~(BIT2 | BIT3);

    EUSCI_A0->CTLW0 |= EUSCI_A_CTLW0_SWRST; // Put eUSCI in reset
    EUSCI_A0->CTLW0 = EUSCI_A_CTLW0_SWRST | // Remain eUSCI in reset
            EUSCI_B_CTLW0_SSEL__SMCLK;      // Configure eUSCI clock source for SMCLK

    // Baud Rate calculation
    // 24000000/(16*115200) = 13.020833333
    // Fractional portion = 0.020833333
    // UCBRFx = int ((13.020833333 - 13)*16) = 0
    EUSCI_A0->BRW = 13;                     // 24000000/16/115200

    // Set the fractional portion of the baud rate and turn
    // on over-sampling
    EUSCI_A0->MCTLW = (0 << EUSCI_A_MCTLW_BRF_OFS) | EUSCI_A_MCTLW_OS16;

    // Enable eUSCI in UART mode
    EUSCI_A0->CTLW0 &= ~EUSCI_A_CTLW0_SWRST;

    // Clear any out standing interrupts
    EUSCI_A0->IFG &= ~(EUSCI_A_IFG_RXIFG | EUSCI_A_IFG_TXIFG);

    // Enable Rx Interrupt
    EUSCI_A0->IE |= EUSCI_A_IE_RXIE;

    // Enable EUSCIA0 Interrupt
    NVIC_EnableIRQ(EUSCIA0_IRQn);

    // Set the priority of the eUSCI to be a value that is equal to 2
    NVIC_SetPriority(EUSCIA0_IRQn, 2);
}

/******************************************************************************
 * This function initializes the eUSCI_A0 hardware by calling console_hw_init().
 * It will also initialize Sem_Console.
 *
 * NOTE: The console window is just for debug purposes for now.
 ******************************************************************************/
void Task_Console_Init(void)
{
    // Initialize the array used to hold UART data
    memset(RX_ARRAY,0,RX_ARRAY_SIZE);

    // Initialize UART
    console_hw_init();

    // Initialize the binary semaphore used to provide mutual exclusion to
    // the UART.
    Sem_Console = xSemaphoreCreateBinary();
    xSemaphoreGive(Sem_Console);

}

/*****************************************************
 * Needed to get printf to work using polling when
 * transmitting characters.
 *****************************************************/
int fputc(int c, FILE* stream)
{
    // While UART is busy, wait
    while(EUSCI_A0->STATW & EUSCI_A_STATW_BUSY){};

    EUSCI_A0->TXBUF = c;

    return 0;
}

//****************************************************************************
// UART interrupt service routine
// ****************************************************************************/
void EUSCIA0_IRQHandler(void)
{
    char c;     // The character received from RXBUF
    BaseType_t xHigherPriorityTaskWoken;

    // Reading from the RXBUF automatically clears the Rx Interrupt
    c = EUSCI_A0->RXBUF;

    // Store character to RX_ARRAY
    RX_ARRAY[RX_INDEX] = c;

    // Once a character has been received, send a Task Notification to Task_Console_Bottom_Half
    vTaskNotifyGiveFromISR(
            Task_Console_Handle,
            &xHigherPriorityTaskWoken
    );

    // Context switch, if needed
    portYIELD_FROM_ISR( xHigherPriorityTaskWoken );

    // Increase the index after a new character is stored
    RX_INDEX++;
}

/******************************************************************************
* Task used to send command from the console window to task_breaker.
******************************************************************************/
void Task_Console_Bottom_Half(void *pvParameters)
{
    TANK_CMD_t message;     // Message prepared to be sent
    bool send_message = true;       // Send Message to Queue_Breaker only when this is true

    while(1)
    {
        // Wait until waken by Top Half
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        // Examine the console if a game is on going
        if(game_on_going)
        {
            // Examine the command type (case insensitive)
            switch(RX_ARRAY[0]){
                case 'a':       // Go Left
                {
                    send_message = true;
                    message = TANK_CMD_LEFT;
                    break;
                }
                case 'A':       // Go Left
                {
                    send_message = true;
                    message = TANK_CMD_LEFT;
                    break;
                }
                case 'd':       // Go Right
                {
                    send_message = true;
                    message = TANK_CMD_RIGHT;
                    break;
                }
                case 'D':       // Go Right
                {
                    send_message = true;
                    message = TANK_CMD_RIGHT;
                    break;
                }
                case 's':       // Go Down
                {
                    send_message = true;
                    message = TANK_CMD_DOWN;
                    break;
                }
                case 'S':       // Go Down
                {
                    send_message = true;
                    message = TANK_CMD_DOWN;
                    break;
                }
                case 'w':       // Go Up
                {
                    send_message = true;
                    message = TANK_CMD_UP;
                    break;
                }
                case 'W':       // Go Up
                {
                    send_message = true;
                    message = TANK_CMD_UP;
                    break;
                }
                case ' ':       // Launch Ball
                {
                    send_message = true;
                    message = TANK_CMD_LAUNCH;
                    break;
                }
                default:        // Any other command will result in no message being sent
                    send_message = false;
            }

            // If a message is ready to be sent, send it to Queue_Breaker
            if(send_message)
            {
                xQueueSend(Queue_Breaker, &message, portMAX_DELAY);
            }
        }

        // Reset RX_ARRAY and RX_INDEX
        memset(RX_ARRAY, 0, RX_ARRAY_SIZE);
        RX_INDEX = 0;
    }
}

