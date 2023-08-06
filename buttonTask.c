/*
 * buttonTask.c
 *
 *  Created on: 7/08/2023
 *      Author: James Laws
 */


/**
* The stack size for the buttons task
**/
#define BUTTONSTASKSTACKSIZE    32         // Stack size in words


/**
* This task reads the buttons' state and puts this information in the buttonInputQueue
**/
static void buttonTask(void *pvParameters) {

    while(1)
    {
        /*
        *   BUTTON READING CODE HERE
        */

        vTaskDelay(pdMS_TO_TICKS(FREQUENCY_BUTTONS_TASK));
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
    if(pdTRUE != xTaskCreate(buttonTask, "buttonTask", BUTTONSTASKSTACKSIZE, NULL, tskIDLE_PRIORITY +
                   PRIORITY_SWITCH_TASK, NULL))
    {
        return(1); // error creating task, out of memory?
    }

    //
    // Success.
    //
    return(0);
}
