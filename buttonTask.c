/*
 * buttonTask.c
 *
 *  Created on: 7/08/2023
 *      Author: James Laws
 */


#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/rom.h"
#include "drivers/buttons.h"
#include "utils/uartstdio.h"
#include "config.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

 /**
 *The item size and queue size for the button input queue.
 **/
 #define BUTTON_INPUT_ITEM_SIZE           sizeof(uint8_t)
 #define BUTTON_INPUT_QUEUE_SIZE          5

/**
* The stack size for the buttons task
**/
#define BUTTON_TASK_STACK_SIZE    32         // Stack size in words



/**
* The queue that holds button inputs
**/
QueueHandle_t buttonInputQueue;


/**
* Gets the button input queue
**/
QueueHandle_t getButtonInputQueue() {
    return buttonInputQueue;
}

/**
* This task reads the buttons' state and puts this information in the buttonInputQueue
**/
static void buttonTask(void *pvParameters) {

    while(1)
    {
        /*
        *   BUTTON READING CODE HERE
        */
        UARTprintf("\n\n Button Input Task");

        vTaskDelay(pdMS_TO_TICKS(FREQUENCY_BUTTON_TASK));
    }
}


/**
* Initializes the buttons task
**/
uint32_t buttonTaskInit(void)
{

    /*
    * Initialize the buttons
    */
    ButtonsInit();

    /*
    * Create the buttons task.
    */
    if(pdTRUE != xTaskCreate(buttonTask, "buttonTask", BUTTON_TASK_STACK_SIZE, NULL, tskIDLE_PRIORITY +
                             PRIORITY_BUTTON_TASK, NULL))
    {
        return(1); // error creating task, out of memory?
    }


    // Create a queue for storing button inputs
    buttonInputQueue = xQueueCreate(BUTTON_INPUT_QUEUE_SIZE, BUTTON_INPUT_ITEM_SIZE);


    // Success.
    return(0);
}
