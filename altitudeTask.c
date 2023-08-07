/*
 * altitudeTask.c
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
* The stack size for the buttons task
**/
#define ALTITUDESTASKSTACKSIZE    32         // Stack size in words


/**
*The item size and queue size for the altitude input queue.
**/
#define ALTITUDE_INPUT_ITEM_SIZE           sizeof(uint8_t)
#define ALTITUDE_INPUT_QUEUE_SIZE          5


/**
* The queue that holds button inputs
**/
QueueHandle_t altitudeInputQueue;


/**
* Gets the altitude input queue
**/
QueueHandle_t getAltitudeInputQueue() {
   return altitudeInputQueue;
}


/**
* This task reads the altitude and puts this information in the altitudenputQueue
**/
static void altitudeTask(void *pvParameters) {

    while(1)
    {
        vTaskDelay(pdMS_TO_TICKS(FREQUENCY_ALTITUDE_TASK));
        /*
        *   altitude READING CODE HERE
        */

        xSemaphoreTake(g_pUARTSemaphore, portMAX_DELAY);
        UARTprintf("\n\n Altitude Input Task");
        xSemaphoreGive(g_pUARTSemaphore);
    }
}


/**
* Initializes the altitude task
**/
uint8_t altitudeTaskInit(void)
{

    /*
    * Create the altitude task.
    */
    if(pdTRUE !=  xTaskCreate(altitudeTask, "altitudeTask", ALTITUDESTASKSTACKSIZE, NULL, tskIDLE_PRIORITY +
                              PRIORITY_ALTITUDE_TASK, NULL))
    {
        return(1); // error creating task, out of memory?
    }

    // Create a queue for storing altitude inputs
    altitudeInputQueue = xQueueCreate(ALTITUDE_INPUT_QUEUE_SIZE, ALTITUDE_INPUT_ITEM_SIZE);

    // Success.
    return(0);
}
