/*
 * altitudeTask.c
 *
 *  Created on: 7/08/2023
 *      Author: James Laws
 */


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
xQueueHandle altitudeInputQueue;


/**
* Gets the altitude input queue
**/
xQueueHandle getAltiduteInputQueue() {
   return altitudInputQueue;
}


/**
* This task reads the altitude and puts this information in the altitudenputQueue
**/
static void altitudeTask(void *pvParameters) {

    while(1)
    {
        /*
        *   altitude READING CODE HERE
        */

        // Runs every 500 millisecond
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}


/**
* Initializes the altitude task
**/
uint32_t altitudeTaskInit(void)
{

    /*
    * Create the altitude task.
    */
    if(pdTRUE !=  xTaskCreate(buttonTask, "altitudeTask", BUTTONSTASKSTACKSIZE, NULL, tskIDLE_PRIORITY +
                   PRIORITY_SWITCH_TASK, NULL))
    {
        return(1); // error creating task, out of memory?
    }

    // Create a queue for storing altitude inputs
    altitudeInputQueue = xQueueCreate(ALTITUDE_INPUT_QUEUE_SIZE, ALTITUDE_INPUT_ITEM_SIZE);

    // Success.
    return(0);
}
