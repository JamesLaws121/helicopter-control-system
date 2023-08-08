/*
 * buttonTask.c
 *
 *  Created on: 7/08/2023
 *      Author: James Laws
 */

#include "FreeRTOS.h"
#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/gpio.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "drivers/buttons.h"
#include "utils/uartstdio.h"
#include "config.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"




 /**
 *The item size and queue size for the button input queue.
 **/
 #define BUTTON_INPUT_ITEM_SIZE           sizeof(uint8_t)
 #define BUTTON_INPUT_QUEUE_SIZE          5

/**
* The stack size for the buttons task
**/
#define BUTTON_TASK_STACK_SIZE    32         // Stack size in words



/**
* The queue that holds button inputs
**/
QueueHandle_t buttonInputQueue;


extern SemaphoreHandle_t g_pUARTSemaphore;


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

    uint32_t ui16LastTime;
    uint32_t ui32SwitchDelay = 25;
    uint8_t ui8CurButtonState, ui8PrevButtonState;
    uint8_t ui8Message;

    ui8CurButtonState = ui8PrevButtonState = 0;

//    ui16LastTime = xTaskGetTickCount();

    while(1)
    {
        vTaskDelay(pdMS_TO_TICKS(FREQUENCY_BUTTON_TASK));
        UARTprintf("In the while loop.\n");

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

                    //
                    // Guard UART from concurrent access.
                    //
                    xSemaphoreTake(g_pUARTSemaphore, portMAX_DELAY);
                    UARTprintf("Left Button is pressed.\n");
                    xSemaphoreGive(g_pUARTSemaphore);
                }
                else if((ui8CurButtonState & ALL_BUTTONS) == RIGHT_BUTTON)
                {
                    ui8Message = RIGHT_BUTTON;

                    //
                    // Guard UART from concurrent access.
                    //
                    xSemaphoreTake(g_pUARTSemaphore, portMAX_DELAY);
                    UARTprintf("Right Button is pressed.\n");
                    xSemaphoreGive(g_pUARTSemaphore);
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

//        vTaskDelay(pdMS_TO_TICKS(FREQUENCY_BUTTON_TASK));


//        xSemaphoreTake(g_pUARTSemaphore, portMAX_DELAY);
//        UARTprintf("\n\n Button Input Task");
//        xSemaphoreGive(g_pUARTSemaphore);
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
    if(pdTRUE != xTaskCreate(buttonTask, "buttonTask", BUTTON_TASK_STACK_SIZE, NULL, tskIDLE_PRIORITY +
                             PRIORITY_BUTTON_TASK, NULL))
    {
        return(1); // error creating task, out of memory?
    }


    // Create a queue for storing button inputs
    buttonInputQueue = xQueueCreate(BUTTON_INPUT_QUEUE_SIZE, BUTTON_INPUT_ITEM_SIZE);


    // Success.
    return(0);
}
