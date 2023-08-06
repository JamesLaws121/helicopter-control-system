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
* This task reads the buttons' state and puts this information in the buttonInputQueue
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

    //
    // Success.
    //
    return(0);
}
