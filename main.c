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
#include "inc/tm4c123gh6pm.h"
#include "driverlib/adc.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/pwm.h"
#include "driverlib/sysctl.h"
#include <FreeRTOS.h>
#include <task.h>

#include "heightController.h"
#include "buttonTask.h"



/**
 * main.c
 */
int main(void)
{
    // Set the clock rate to 80 MHz
    SysCtlClockSet (SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
     SYSCTL_XTAL_16MHZ);


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

    vTaskStartScheduler(); // Start FreeRTOS!!

    while(1);
}



void vAssertCalled( const char * pcFile, unsigned long ulLine ) {
    (void) pcFile;
    (void) ulLine;
    while (1) ;
}
