/*
 * HeightController.c
 *
 *  Created on: 2/08/2023
 *      Author: James Laws
 */

#include <FreeRTOS.h>
#include <stdio.h>
#include <task.h>

#include "buttonTask.h"
#include "altitudeTask.h"
#include "heightOuput.h"


/**
* The stack size for the buttons task
**/
#define HEIGHTTASKSTACKSIZE    32         // Stack size in words


/**
* This is the current height set by the user
**/
static helicopterHeight = 0;



/**
* This task monitors and alters the helicopters height
**/
static void heightControllerTask(void *pvParameters) {

    uint8_t buttonInputMessage;

    uint8_t altitudeInputMessage;

    uint8_t heightOuputMessage;

    while(1)
    {
        xQueueHandle altitudeInputQueue = getAltiduteInputQueue();

        xQueueHandle buttonInputQueue = getButtonInputQueue();


        // Read the next button input, if available on queue.
        if(xQueueReceive(buttonInputQueue, &buttonInputMessage, 0) == pdPASS) {
            // Update height based on button buttonInput
            helicopterHeight = buttonInputMessage;
        }


        // Reads the altitude input and updates output accordingly
        if (xQueueReceive(altitudeInputQueue, &altitudeInputMessage, 0) == pdPASS) {
            // Calculate rotar output to get to wanted altitude
            heightOuputMessage = 1;

            xQueueHandle heightOutputQueue = getHeightOutputQueue();
            // Write to ouput queue
            if(xQueueSend(heightOutputQueue, &heightOuputMessage , portMAX_DELAY) != pdPASS) {
                // Error. The queue should never be full.
                UARTprintf("\nQueue full. This should never happen.\n");
                while(1)
                {
                }
            }
        }



        vTaskDelay(pdMS_TO_TICKS(FREQUENCY_HEIGHT_CONTROLLER_TASK));
    }
}

/**
 * Initializes the Height controller
 */
void heightControllerInit() {

    /*
    * Create the buttons task.
    */
    if(pdTRUE !=  xTaskCreate(heightControllerTask, "heightControllerTask", HEIGHTTASKSTACKSIZE, NULL, tskIDLE_PRIORITY +
                   PRIORITY_HEIGHT_CONTROLLER_TASK, NULL))
    {
        return(1); // error creating task, out of memory?
    }

    return 0;
}
