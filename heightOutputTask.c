/*
 * heightOuputTask.c
 *
 *  Created on: 7/08/2023
 *  Edited on: 12/08/2023
 *      Author: James Laws
 *      Editor: AJ Seville
 */


#include <stdbool.h>
#include <stdint.h>

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/rom.h"
#include "driverlib/pwm.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"

#include "drivers/buttons.h"
#include "drivers/uartstdio.h"
#include "drivers/motor.h"

#include "FreeRTOS.h"
#include "timers.h"
#include "config.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "heightController.h"







/**
* The stack size for the heightOuput task
**/
#define HEIGHT_OUTPUT_TASK_STACK_SIZE    32         // Stack size in words

/**
*The item size and queue size for the height output queue.
**/
#define HEIGHT_OUTPUT_ITEM_SIZE           sizeof(HeightStructure_t)
#define HEIGHT_OUTPUT_QUEUE_SIZE          1

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
        vTaskDelay(pdMS_TO_TICKS(FREQUENCYY_HEIGHT_OUTPUT_TASK));
        /*
        *   HEIGHT output READING CODE HERE
        */

        xSemaphoreTake(UARTSemaphore, portMAX_DELAY);
        UARTprintf("\n\n Height Output Task");
        xSemaphoreGive(UARTSemaphore);
    }
}


/**
* Initializes the heightOuput task
**/
uint32_t heightOutputTaskInit(void)
{

    // Create a queue for storing height
    heightOutputQueue = xQueueCreate(HEIGHT_OUTPUT_QUEUE_SIZE, HEIGHT_OUTPUT_ITEM_SIZE);

    /*
    * Create the heightOuput task.
    */
    if(pdTRUE != xTaskCreate(heightOuputTask, "heightOuputTask", HEIGHT_OUTPUT_TASK_STACK_SIZE, NULL, tskIDLE_PRIORITY +
                   PRIORITY_HEIGHT_OUTPUT_TASK, NULL))
    {
        return(1); // error creating task, out of memory?
    }








    // Success.
    return(0);
}





