/*
 * uartSetup.c
 *
 *  Created on: 15/08/2023
 *      Author: jla201
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

#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "config.h"


/**
 * The semaphore that protects concurrent access of UART from multiple tasks.
 */
SemaphoreHandle_t UARTSemaphore;


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

uint8_t setupUart(void) {
    // Initialize the UART and configure it for 115,200, 8-N-1 operation.
    ConfigureUART();

    // Create a binary semaphore to guard the UART.
    UARTSemaphore = xSemaphoreCreateBinary();
    while ( UARTSemaphore == NULL ) {

    }
    xSemaphoreGive(UARTSemaphore);
    UARTprintf("\n\nWelcome to project");

    return 0;
}
