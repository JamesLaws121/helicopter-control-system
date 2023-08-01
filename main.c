/*
 * main.c
 *
 *  Created on: 2/08/2023
 *      Authors: James Laws, Ben, AJ, Tom
 */




#include <FreeRTOS.h>
#include <task.h>
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

#include "analogueConverter.h"


//static void BlinkLED(void *);

static void readAnologueInput(void *);



/**
 * main.c
 */
int main(void)
{
    // Set the clock rate to 80 MHz
    SysCtlClockSet (SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
     SYSCTL_XTAL_16MHZ);



    // For LED blinky task - initialize GPIO port F and then pin #1 (red) for
//    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
//    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF)) ; // busy-wait until
//
//    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1); // PF_1 as output
//    // doesn't need too much drive strength as the RGB LEDs on the TM4C123 launchpad are switched via N-type transistors
//
//    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_STRENGTH_4MA,GPIO_PIN_TYPE_STD);
//    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0); // off by default

//    if (pdTRUE != xTaskCreate(BlinkLED, "Blinker", 32, (void *)1, 4, NULL))
//    { // (void *)1 is our pvParameters for our task func specifying PF_1
//     while (1) ; // error creating task, out of memory?
//    }
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_4); // PE_4 Altitude input
    GPIO_PORTE_LOCK_R = GPIO_LOCK_KEY;
    GPIO_PORTE_CR_R |= GPIO_PIN_4;
    GPIO_PORTE_LOCK_R = GPIO_LOCK_M;

    initADC();


    //HWREG(GPIO_PORTC_BASE + GPIO_E_LOCK) = GPIO_LOCK_KEY;

    //GPIOPadConfigSet(GPIO_PORTE_BASE, GPIO_PIN_4, GPIO_STRENGTH_4MA,GPIO_PIN_TYPE_STD);

    if (pdTRUE != xTaskCreate(readAnologueInput, "Read Task", 32, (void *)1, 4, NULL)) {
         while (1) ; // error creating task, out of memory?
    }

    vTaskStartScheduler(); // Start FreeRTOS!!

    // Should never get here since the RTOS should never "exit".
    while(1);
}




//Blinky function
//void BlinkLED(void *pvParameters) {
// /* While pvParameters is technically a pointer, a pointer is nothing
// * more than an unsigned integer of size equal to the architecture's
// * memory address bus width, which is 32-bits in ARM. We're abusing
// * the parameter then to hold a simple integer value. Could also have
// * used this as a pointer to a memory location holding the value, but
// * our method uses less memory.
// */
// const unsigned int whichLed = (unsigned int)pvParameters;
// // TivaWare GPIO calls require the pin# as a binary bitmask,
// // not a simple number. Alternately, we could have passed the
// // bitmask into pvParameters instead of a simple number.
// const uint8_t whichBit = 1 << whichLed;
// uint8_t currentValue = 0;
// while (1)
// {
// // XOR toggles the bit on/off each time this runs.
//
//
// currentValue ^= whichBit;
//
//
// GPIOPinWrite(GPIO_PORTF_BASE, whichBit, currentValue);
// // Suspend this task (so others may run) for 125ms
// // or as close as we can get with the current RTOS tick setting.
// // (vTaskDelay takes scheduler ticks as its parameter, so use the
// // pdMS_TO_TICKS macro to convert milliseconds to ticks.)
// vTaskDelay(pdMS_TO_TICKS(125));
//
// }
// // No way to kill this blinky task unless another task has an
// // xTaskHandle reference to it and can use vTaskDelete() to purge it.
//}



static void readAnologueInput(void *pvParameters) {

    while(1)
    {
        //int32_t x = GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_4);
        // With this task always delaying, the RTOS Idle Task runs almost
        // all the time.
        uint32_t x = getADCData();
        vTaskDelay(1000);
    }
}



void vAssertCalled( const char * pcFile, unsigned long ulLine ) {
    (void) pcFile;
    (void) ulLine;
    while (1) ;
}
