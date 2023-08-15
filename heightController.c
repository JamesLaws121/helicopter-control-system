/*
 * HeightController.c
 *
 *  Created on: 2/08/2023
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
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "config.h"
#include "buttonTask.h"
#include "heightOutputTask.h"
#include "altitudeTask.h"

 /**
 *The item size and queue size for the calibration queue.
 **/
 #define CALIBRATION_ITEM_SIZE           sizeof(uint8_t)
 #define CALIBRATION_QUEUE_SIZE          1


/**
* The stack size for the buttons task
**/
#define HEIGHT_TASK_STACK_SIZE    128         // Stack size in words


QueueHandle_t calibrationQueue;

QueueHandle_t getCalibrationQueue() {
    return calibrationQueue;
}

/**
* This task monitors and alters the helicopters height
**/
static void heightControllerTask(void *pvParameters) {
    /**
    * This is the current height set by the user
    **/

    uint8_t buttonInputMessage;

    uint16_t altitudeInputMessage;

    uint8_t heightOuputMessage;

    int8_t groundVoltage = -1;

    while(1)
    {
        vTaskDelay(pdMS_TO_TICKS(FREQUENCY_HEIGHT_CONTROLLER_TASK));
        heightOuputMessage = 0;
        xSemaphoreTake(UARTSemaphore, portMAX_DELAY);

        UARTprintf("\n\nHeight Controller Task");


        // Reads the altitude input and updates output accordingly
        QueueHandle_t altitudeInputQueue = getAltitudeInputQueue();
        if (xQueueReceive(altitudeInputQueue, &altitudeInputMessage, 0) == pdPASS) {
            // Calculate roter output to get to wanted altitude
            UARTprintf("\n READ FROM ALTITUDE QUEUE\n RESULT: %d",altitudeInputMessage);
            heightOuputMessage = altitudeInputMessage/10;

            // Calibrate ground voltage
            if (groundVoltage == -1) {
                UARTprintf("\n Setting ground voltage.\n");
                groundVoltage = altitudeInputMessage;

                uint8_t calibrationMessage = 1;
                if(xQueueSendToBack(calibrationQueue, &calibrationMessage , 5) != pdPASS) {
                    UARTprintf("\nERROR: calibration queue full. This should never happen.\n");
                }
            }

        }



          // Write to output queue
//        if ( groundVoltage != -1 && heightOuputMessage != 0) {
//            // Calculations HERE
//            UARTprintf("\n Send data to height output\n");
//
//            QueueHandle_t heightOutputQueue = getHeightOutputQueue();
//            if(xQueueSendToBack(heightOutputQueue, &heightOuputMessage , 5) != pdPASS) {
//                UARTprintf("\nERROR: Queue full. This should never happen.\n");
//            }
//        }


        // Read the next button input, if available on queue.
        QueueHandle_t buttonInputQueue = getButtonInputQueue();
        if(xQueueReceive(buttonInputQueue, &buttonInputMessage, 0) == pdPASS) {
            // Update height based on button buttonInput
            UARTprintf("\n\BUTTON: %d", buttonInputMessage);
        }


        xSemaphoreGive(UARTSemaphore);
    }
}

/**
 * Initializes the Height controller
 */
uint8_t heightControllerInit() {

    // Create a queue for calibrating the ground
    calibrationQueue = xQueueCreate(CALIBRATION_QUEUE_SIZE, CALIBRATION_ITEM_SIZE);

    /*
    * Create the buttons task.
    */
    if(pdTRUE !=  xTaskCreate(heightControllerTask, "heightControllerTask", HEIGHT_TASK_STACK_SIZE, NULL, tskIDLE_PRIORITY +
                   PRIORITY_HEIGHT_CONTROLLER_TASK, NULL))
    {
        return(1); // error creating task, out of memory?
    }

    return 0;
}
