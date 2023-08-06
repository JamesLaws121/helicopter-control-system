/*
 * buttonTask.c
 *
 *  Created on: 7/08/2023
 *      Author: James Laws
 */



 /**
 *The item size and queue size for the button input queue.
 **/
 #define BUTTON_INPUT_ITEM_SIZE           sizeof(uint8_t)
 #define BUTTON_INPUT_QUEUE_SIZE          5

/**
* The stack size for the buttons task
**/
#define BUTTONSTASKSTACKSIZE    32         // Stack size in words



/**
* The queue that holds button inputs
**/
xQueueHandle buttonInputQueue;


/**
* Gets the button input queue
**/
xQueueHandle getButtonInputQueue() {
    return buttonInputQueue;
}

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


    // Create a queue for storing button inputs
    buttonInputQueue = xQueueCreate(BUTTON_INPUT_QUEUE_SIZE, BUTTON_INPUT_ITEM_SIZE);


    // Success.
    return(0);
}
