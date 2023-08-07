/*
 * HeightController.c
 *
 *  Created on: 2/08/2023
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

#include "buttonTask.h"
#include "heightOutputTask.h"
#include "altitudeTask.h"


/**
* The stack size for the buttons task
**/
#define HEIGHT_TASK_STACK_SIZE    32         // Stack size in words


extern SemaphoreHandle_t g_pUARTSemaphore;

/**
* This task monitors and alters the helicopters height
**/
static void heightControllerTask(void *pvParameters) {
    /**
    * This is the current height set by the user
    **/
    int helicopterHeight = 0;

    uint8_t buttonInputMessage;

    uint8_t altitudeInputMessage;

    uint8_t heightOuputMessage;

    while(1)
    {
        vTaskDelay(pdMS_TO_TICKS(FREQUENCY_HEIGHT_CONTROLLER_TASK));


        xSemaphoreTake(g_pUARTSemaphore, portMAX_DELAY);
        UARTprintf("\n\nHeight Controller Task");
        xSemaphoreGive(g_pUARTSemaphore);


        QueueHandle_t altitudeInputQueue = getAltitudeInputQueue();
        QueueHandle_t buttonInputQueue = getButtonInputQueue();


        // Read the next button input, if available on queue.
        if(xQueueReceive(buttonInputQueue, &buttonInputMessage, 0) == pdPASS) {
            // Update height based on button buttonInput
            helicopterHeight = buttonInputMessage;
            UARTprintf("\n\nHeight: %d", helicopterHeight);
        }


        // Reads the altitude input and updates output accordingly
        if (xQueueReceive(altitudeInputQueue, &altitudeInputMessage, 0) == pdPASS) {
            // Calculate roter output to get to wanted altitude
            heightOuputMessage = 1;

            QueueHandle_t heightOutputQueue = getHeightOutputQueue();
            // Write to output queue
            if(xQueueSend(heightOutputQueue, &heightOuputMessage , portMAX_DELAY) != pdPASS) {
                // Error. The queue should never be full.
                UARTprintf("\nQueue full. This should never happen.\n");
                while(1)
                {
                }
            }
        }
    }
}

/**
 * Initializes the Height controller
 */
uint8_t heightControllerInit() {

    /*
    * Create the buttons task.
    */
    if(pdTRUE !=  xTaskCreate(heightControllerTask, "heightControllerTask", HEIGHT_TASK_STACK_SIZE, NULL, tskIDLE_PRIORITY +
                   PRIORITY_HEIGHT_CONTROLLER_TASK, NULL))
    {
        return(1); // error creating task, out of memory?
    }

    return 0;
}
