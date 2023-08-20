/*
 * altitudeTask.c
 *
 *  Created on: 19/08/2023
 *      Author: James Laws, Tom Clifton
 */

#include <stdbool.h>
#include <stdint.h>
#include "FreeRTOS.h"

#include "driverlib/adc.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"

#include "altitudeTask.h"
#include "analogueConverter.h"
#include "config.h"
#include "drivers/uartstdio.h"
#include "queue.h"
#include "task.h"

/**
* The stack size for the buttons task
**/
#define ALTITUDE_TASK_STACK_SIZE    32         // Stack size in words

/**
* The number of samples to trigger before updating the controller
**/
#define SAMPLE_COUNT    5


/**
*The item size and queue size for the altitude input queue.
**/
#define ALTITUDE_INPUT_ITEM_SIZE           sizeof(uint16_t)
#define ALTITUDE_INPUT_QUEUE_SIZE          5


/**
* The queue that holds button inputs
**/
QueueHandle_t altitudeInputQueue;


/**
* Gets the altitude input queue
**/
QueueHandle_t getAltitudeInputQueue(void) {
   return altitudeInputQueue;
}


/**
* This task reads the altitude and puts this information in the altitudenputQueue
**/
static void altitudeTask(void *pvParameters) {
    // Initializes the ADC peripheral
    initADC();

    uint8_t sampleCount = 0;
    while(1)
    {
        vTaskDelay(pdMS_TO_TICKS(FREQUENCY_ALTITUDE_TASK));

        xSemaphoreTake(UARTSemaphore, portMAX_DELAY);

        // Trigger the collection of altitude data
        ADCProcessorTrigger(ADC0_BASE, 3);
        sampleCount ++;

        // Sends the height controller an average
        if (sampleCount == SAMPLE_COUNT) {
            sampleCount = 0;
            uint16_t sampleAverage = getBufferMean();
            
            if(xQueueSendToBack(altitudeInputQueue, &sampleAverage , 2) != pdPASS) {
                UARTprintf("\nERROR: Queue full. This should never happen.\n");
            }

        }
        xSemaphoreGive(UARTSemaphore);
        
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
    if(pdTRUE !=  xTaskCreate(altitudeTask, "altitudeTask", ALTITUDE_TASK_STACK_SIZE, NULL, tskIDLE_PRIORITY +
                              PRIORITY_ALTITUDE_TASK, NULL))
    {
        return(1); // error creating task, out of memory?
    }

    // Create a queue for storing altitude inputs
    altitudeInputQueue = xQueueCreate(ALTITUDE_INPUT_QUEUE_SIZE, ALTITUDE_INPUT_ITEM_SIZE);

    // Success.
    return(0);
}
