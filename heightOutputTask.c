/*
 * heightOuputTask.c
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
* The stack size for the heightOuput task
**/
#define HEIGHT_OUTPUT_TASK_STACK_SIZE    32         // Stack size in words

/**
*The item size and queue size for the height ouput queue.
**/
#define HEIGHT_OUTPUT_ITEM_SIZE           sizeof(uint8_t)
#define HEIGHT_OUTPUT_QUEUE_SIZE          5


/**
* The queue that holds button inputs
**/
QueueHandle_t heightOutputQueue;

/**
* Gets the heightOuput queue
**/
QueueHandle_t getHeightOutputQueue() {
   return heightOutputQueue;
}


/**
* This task reads the heightOuputQueue and outputs the desired output
**/
static void heightOuputTask(void *pvParameters) {

    while(1)
    {
        /*
        *   HEIGHT output READING CODE HERE
        */
        UARTprintf("\n\n Height Output Task");

        vTaskDelay(pdMS_TO_TICKS(FREQUENCYY_HEIGHT_OUTPUT_TASK));
    }
}


/**
* Initializes the heightOuput task
**/
uint32_t heightOuputTaskInit(void)
{

    /*
    * Create the heightOuput task.
    */
    if(pdTRUE != xTaskCreate(heightOuputTask, "heightOuputTask", HEIGHT_OUTPUT_TASK_STACK_SIZE, NULL, tskIDLE_PRIORITY +
                   PRIORITY_HEIGHT_OUTPUT_TASK, NULL))
    {
        return(1); // error creating task, out of memory?
    }

    // Create a queue for storing height
    heightOutputQueue = xQueueCreate(HEIGHT_OUTPUT_QUEUE_SIZE, HEIGHT_OUTPUT_ITEM_SIZE);

    // Success.
    return(0);
}
