/*
 * buttonTask.c
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

#include "heightController.h"

 /**
 *The item size and queue size for the button input queue.
 **/
 #define BUTTON_INPUT_ITEM_SIZE           sizeof(uint8_t)
 #define BUTTON_INPUT_QUEUE_SIZE          5

/**
* The stack size for the buttons task
**/
#define BUTTON_TASK_STACK_SIZE    128         // Stack size in words



/**
* The queue that holds button inputs
**/
QueueHandle_t buttonInputQueue;


/**
* Gets the button input queue
**/
QueueHandle_t getButtonInputQueue() {
    return buttonInputQueue;
}

/**
* This task reads the buttons' state and puts this information in the buttonInputQueue
**/
static void buttonTask(void *pvParameters) {
    uint8_t calibration_state = 0;

    while(1)
    {
        vTaskDelay(pdMS_TO_TICKS(FREQUENCY_BUTTON_TASK));

        xSemaphoreTake(UARTSemaphore, portMAX_DELAY);
        UARTprintf("\n\n Button Input Task");
        xSemaphoreGive(UARTSemaphore);


        if (calibration_state == 0) {
            // Don't take user input until calibration finished
            UARTprintf("\n\n Calibrating");
            QueueHandle_t calibrationQueue = getCalibrationQueue();
            xQueuePeek( calibrationQueue, &calibration_state, 0 );
            if (calibration_state == 1) {
                UARTprintf("\n\n Finished Calibrating");
            } else {
                UARTprintf("\n\n Still Calibrating");
            }
            continue;
        }

        /*
        *   BUTTON READING CODE HERE
        */

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

    // Create a queue for storing button inputs
    buttonInputQueue = xQueueCreate(BUTTON_INPUT_QUEUE_SIZE, BUTTON_INPUT_ITEM_SIZE);

    /*
    * Create the buttons task.
    */
    if(pdTRUE != xTaskCreate(buttonTask, "buttonTask", BUTTON_TASK_STACK_SIZE, NULL, tskIDLE_PRIORITY +
                             PRIORITY_BUTTON_TASK, NULL))
    {
        return(1); // error creating task, out of memory?
    }


    // Success.
    return(0);
}
