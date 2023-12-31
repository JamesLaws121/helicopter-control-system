/*
 * buttonTask.c
 *
 *  Created on: 7/08/2023
 *      Authors: Benjamin Stewart, James Laws
 */

#include <stdbool.h>
#include <stdint.h>
#include "driverlib/gpio.h"
#include "FreeRTOS.h"
#include "inc/hw_memmap.h"

#include "buttonTask.h"
#include "config.h"
#include "drivers/buttons.h"
#include "drivers/uartstdio.h"
#include "heightController.h"
#include "queue.h"
#include "semphr.h"
#include "task.h"


 /**
 *The item size and queue size for the button input queue.
 **/
 #define BUTTON_INPUT_ITEM_SIZE           sizeof(uint8_t)
 #define BUTTON_INPUT_QUEUE_SIZE          5

#define portTICK_RATE_MS 1000

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
QueueHandle_t getButtonInputQueue(void) {
    return buttonInputQueue;
}

/**
* This task reads the buttons' state and puts this information in the buttonInputQueue
**/
static void buttonTask(void *pvParameters) {
    uint8_t calibration_state = 0;

    uint8_t ui8CurButtonState, ui8PrevButtonState;
    uint8_t ui8Message;

    ui8CurButtonState = ui8PrevButtonState = 0;

    while (calibration_state == 0) {
        vTaskDelay(pdMS_TO_TICKS(CALIBRATION_FREQUENCY + FREQUENCY_BUTTON_TASK));
        // Don't take user input until calibration finished
        QueueHandle_t calibrationQueue = getCalibrationQueue();
        xQueuePeek( calibrationQueue, &calibration_state, 0 );
    }

    while(1)
    {


        vTaskDelay(pdMS_TO_TICKS(FREQUENCY_BUTTON_TASK));

        xSemaphoreTake(UARTSemaphore, portMAX_DELAY);

        ui8CurButtonState = ButtonsPoll(0, 0);

        //
        // Check if previous debounced state is equal to the current state.
        //
        if(ui8CurButtonState != ui8PrevButtonState)
        {
            ui8PrevButtonState = ui8CurButtonState;

            //
            // Check to make sure the change in state is due to button press
            // and not due to button release.
            //
            if((ui8CurButtonState & ALL_BUTTONS) != 0)
            {
                if((ui8CurButtonState & ALL_BUTTONS) == LEFT_BUTTON)
                {
                    ui8Message = LEFT_BUTTON;
                    //Put actual functionality here
                    UARTprintf("\n\nLeft button pressed");
                }
                else if((ui8CurButtonState & ALL_BUTTONS) == RIGHT_BUTTON)
                {
                    ui8Message = RIGHT_BUTTON;

                    //Put actual functionality here
                    UARTprintf("\n\nRight button pressed");
                }

                //
                // Pass the value of the button pressed to LEDTask.
                //
                if(xQueueSend(buttonInputQueue, &ui8Message, portMAX_DELAY) !=
                   pdPASS)
                {
                    //
                    // Error. The queue should never be full. If so print the
                    // error message on UART and wait for ever.
                    //
                    UARTprintf("\nQueue full. This should never happen.\n");
                    while(1)
                    {
                    }
                }
            }
        }
        xSemaphoreGive(UARTSemaphore);
    }
}


/**
* Initializes the buttons task
**/
uint8_t buttonTaskInit(void)
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
    if(pdTRUE != xTaskCreate(buttonTask, (const portCHAR *)"buttonTask", BUTTON_TASK_STACK_SIZE, NULL, tskIDLE_PRIORITY +
                             PRIORITY_BUTTON_TASK, NULL))
    {
        return(1); // error creating task, out of memory?
    }


    // Success.
    return(0);
}
