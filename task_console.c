/*
 * task_console.c
 *
 *  Created on: Oct 21, 2020
 *      Author: Joe Krachey
 *
 *  Student Name: Justin Qiao
 */

#include <main.h>

QueueHandle_t Queue_Console;
TaskHandle_t Task_Console_Handle;
SemaphoreHandle_t Sem_Console;

#define RX_ARRAY_SIZE 10

// Global variables used to store incoming data from RXBUF.
volatile char RX_ARRAY[RX_ARRAY_SIZE];
volatile uint16_t RX_INDEX=0;

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


