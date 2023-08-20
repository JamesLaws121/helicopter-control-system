/*
 * main.c
 *
 *  Created on: 2/08/2023
 *      Authors: James Laws, Benjamin Stewart, AJ Seville, Thomas Clifton
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

#include "FreeRTOS.h"
#include "drivers/uartstdio.h"
#include "drivers/motor.h"


#include "task.h"
#include "queue.h"
#include "semphr.h"


#include "buttonTask.h"
#include "heightOutputTask.h"
#include "altitudeTask.h"
#include "heightController.h"
#include "uartSetup.h"


/**
 * main.c
 */
int main(void)
{
    // Set the clock rate to 80 MHz
    SysCtlClockSet (SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
     SYSCTL_XTAL_16MHZ);


    if(setupUart() != 0)
    {
        // Error while setting up UART
        while(1)
        {
        }
    }

    UARTprintf("\n\nWelcome to project");



    /*
    * Create the button task
    */
    if(buttonTaskInit() != 0)
    {
        // Error while creating task
        while(1)
        {
            UARTprintf("\n\nERROR creating button task");
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
            UARTprintf("\n\nERROR creating button task");
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
            UARTprintf("\n\nERROR creating button task");
        }
    }

    /*
    * Create the heightOuput task
    */
    if(heightOutputTaskInit() != 0)
    {
        // Error while creating task
        while(1)
        {
            UARTprintf("\n\nERROR creating button task");
        }
    }




    vTaskStartScheduler(); // Start FreeRTOS!!


    // Error while creating tasks
    while(1)
    {
        UARTprintf("\n\nERROR creating height Controller task");
    }

}


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


void vAssertCalled( const char * pcFile, unsigned long ulLine ) {
    (void) pcFile;
    (void) ulLine;
    while (1) ;
}
