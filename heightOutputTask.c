/*
 * heightOuputTask.c
 *
 *  Created on: 7/08/2023
 *      Author: James Laws
 */


/**
* The stack size for the heightOuput task
**/
#define HEIGHTOUPUTSTASKSTACKSIZE    32         // Stack size in words

/**
*The item size and queue size for the height ouput queue.
**/
#define HEIGHT_OUTPUT_ITEM_SIZE           sizeof(uint8_t)
#define HEIGHT_OUTPUT_QUEUE_SIZE          5


/**
* The queue that holds button inputs
**/
xQueueHandle heightOutputQueue;

/**
* Gets the heightOuput queue
**/
xQueueHandle getheightOuputQueue() {
   return heightOuputQueue;
}


/**
* This task reads the heightOuputQueue and outputs the desired output
**/
static void heightOuputTask(void *pvParameters) {

    while(1)
    {
        /*
        *   HEIGHT ouput READING CODE HERE
        */

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
    if(pdTRUE != xTaskCreate(buttonTask, "heightOuputTask", HEIGHTOUPUTSTASKSTACKSIZE, NULL, tskIDLE_PRIORITY +
                   PRIORITY_HEIGHT_OUTPUT_TASK, NULL))
    {
        return(1); // error creating task, out of memory?
    }

    // Create a queue for storing height
    altitudeInputQueue = xQueueCreate(ALTITUDE_INPUT_QUEUE_SIZE, ALTITUDE_INPUT_ITEM_SIZE);

    // Success.
    return(0);
}
