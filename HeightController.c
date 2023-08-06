/*
 * HeightController.c
 *
 *  Created on: 2/08/2023
 *      Author: James Laws
 */

#include <FreeRTOS.h>
#include <stdio.h>
#include <task.h>


/**
* The stack size for the buttons task
**/
#define HEIGHTTASKSTACKSIZE    32         // Stack size in words


/**
* This task monitors and alters the helicopters height
**/
static void heightControllerTask(void *pvParameters) {

    while(1)
    {

        // Read altitude data

        // Compare with wanted altitude

        // Calculate rotar output to get to wanted altitude

        // Write to ouput queue

        // Runs every 500 millisecond
        vTaskDelay(pdMS_TO_TICKS(500));
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
