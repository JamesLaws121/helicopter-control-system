/*
 * main.c
 *
 *  Created on: 2/08/2023
 *      Authors: James Laws, Ben, AJ, Tom
 */




#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "buttonTask.h"
#include "heightOutputTask.h"
#include "altitudeTask.h"
#include "heightController.h"



/**
 * The mutex that protects concurrent access of UART from multiple tasks.
 */
SemaphoreHandle_t g_pUARTSemaphore;


/**
 * This hook is called by FreeRTOS when an stack overflow error is detected.
 */
void vApplicationStackOverflowHook(TaskHandle_t *pxTask, char *pcTaskName)
{
    //
    // This function can not return, so loop forever.  Interrupts are disabled
    // on entry to this function, so no processor interrupts will interrupt
    // this loop.
    //
    while(1)
    {
    }
}


/**
 * Configure the UART and its pins.  This must be called before UARTprintf().
 */
void ConfigureUART(void)
{
    //
    // Enable the GPIO Peripheral used by the UART.
    //
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    //
    // Enable UART0
    //
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    //
    // Configure GPIO Pins for UART mode.
    //
    ROM_GPIOPinConfigure(GPIO_PA0_U0RX);
    ROM_GPIOPinConfigure(GPIO_PA1_U0TX);
    ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    //
    // Use the internal 16MHz oscillator as the UART clock source.
    //
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);

    //
    // Initialize the UART for console I/O.
    //
    UARTStdioConfig(0, 115200, 16000000);
}


/**
 * main.c
 */
int main(void)
{
    // Set the clock rate to 80 MHz
    SysCtlClockSet (SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
     SYSCTL_XTAL_16MHZ);

    // Initialize the UART and configure it for 115,200, 8-N-1 operation.
    ConfigureUART();


    UARTprintf("\n\nWelcome to project");


    // Create a mutex to guard the UART.
    g_pUARTSemaphore = xSemaphoreCreateMutex();


    /*
    * Create the button task
    */
    if(buttonTaskInit() != 0)
    {
        // Error while creating task
        while(1)
        {
        }
    }

    /*
    * Create the altitude task
    */
    if(altitudeTaskInit() != 0)
    {
        // Error while creating task
        while(1)
        {
        }
    }

    /*
    * Create the heightOuput task
    */
    if(heightOuputTaskInit() != 0)
    {
        // Error while creating task
        while(1)
        {
        }
    }

    /*
    * Create the heightController task
    */
    if(heightControllerInit() != 0)
    {
        // Error while creating task
        while(1)
        {
        }
    }

    vTaskStartScheduler(); // Start FreeRTOS!!

    while(1);
}



void vAssertCalled( const char * pcFile, unsigned long ulLine ) {
    (void) pcFile;
    (void) ulLine;
    while (1) ;
}
