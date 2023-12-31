/*
 * heightController.c
 *
 *  Created on: 2/08/2023
 *      Author: James Laws, Benjamin Stewart
 */


#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"

#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "task.h"

#include "drivers/uartstdio.h"

#include "config.h"
#include "altitudeOutputTask.h"
#include "altitudeTask.h"
#include "buttonTask.h"
#include "heightController.h"



 /**
 *The item size and queue size for the calibration queue.
 **/
#define CALIBRATION_ITEM_SIZE           sizeof(uint8_t)
#define CALIBRATION_QUEUE_SIZE          1

 /**
 *The max altitude value
 **/
#define MAX_HEIGHT 1300

/**
* The stack size for the buttons task
**/
#define HEIGHT_TASK_STACK_SIZE    256         // Stack size in words


/**
* The queue that holds the calibration state
**/
QueueHandle_t calibrationQueue;


/**
* Gets the calibration state queue
**/
QueueHandle_t getCalibrationQueue(void) {
    return calibrationQueue;
}


/**
* This task monitors and alters the helicopters height
**/
static void heightControllerTask(void *pvParameters) {
    /**
    * This is the current height set by the user
    **/

    uint8_t changeInState = 1;
    uint16_t newHeight = 0;

    uint8_t buttonInputMessage;
    uint16_t altitudeInputMessage;

    HeightStructure_t heightStatus;
    heightStatus.currentHeight = 0;
    heightStatus.desiredHeight = 0;


    uint32_t groundVoltage = 0;

    uint8_t calibrationCountdown = 4;

    while(1)
    {
        vTaskDelay(pdMS_TO_TICKS(FREQUENCY_HEIGHT_CONTROLLER_TASK));
        xSemaphoreTake(UARTSemaphore, portMAX_DELAY);


        // Read and adjust height based on all button inputs, if any available on queue.
        QueueHandle_t buttonInputQueue = getButtonInputQueue();
        while(xQueueReceive(buttonInputQueue, &buttonInputMessage, 0) == pdPASS) {
            heightStatus.desiredHeight = calculateNewHeight(heightStatus.desiredHeight, buttonInputMessage);
            changeInState = 1;
        }

        // Reads the altitude input and updates output accordingly
        QueueHandle_t altitudeInputQueue = getAltitudeInputQueue();

        if (xQueueReceive(altitudeInputQueue, &altitudeInputMessage, 0) == pdPASS) {

            // Calibrate ground voltage
            if (calibrationCountdown != 0) {
                calibrationCountdown -= 1;
                UARTprintf("\n Calibrating \n");

                if (calibrationCountdown != 3) {
                    groundVoltage = groundVoltage==0? altitudeInputMessage: (groundVoltage + altitudeInputMessage)/2;
                }

                if (calibrationCountdown == 0) {
                    UARTprintf("\n Calibrating Finished \n");

                    uint8_t calibrationMessage = 1;
                    xQueueOverwrite(calibrationQueue, &calibrationMessage);
                }
            } else {
                // Calculate current height
                newHeight = (altitudeInputMessage > groundVoltage) ? 0 : groundVoltage - altitudeInputMessage;
                if (heightStatus.currentHeight != newHeight) {
                    // Used to tell if the height has changed
                    changeInState = 1;
                    heightStatus.currentHeight = newHeight;
                }

                // Write to output queue
                QueueHandle_t altitudeOutputQueue = getAltitudeOutputQueue();
                xQueueOverwrite(altitudeOutputQueue, &heightStatus);
            }

        }


        /**
         * Displays current height and altitude data to the user
         */
        if (changeInState == 1 && calibrationCountdown == 0) {
            changeInState = 0;
            UARTprintf("\n CURRENT HEIGHT: %d mv",heightStatus.currentHeight);
            UARTprintf("\n DESIRED HEIGHT: %d mv",heightStatus.desiredHeight);
            UARTprintf("\n CURRENT HEIGHT PERCENTAGE: %d %%",(heightStatus.currentHeight)/(MAX_HEIGHT/100));
            UARTprintf("\n DESIRED HEIGHT PERCENTAGE: %d %% \n",(heightStatus.desiredHeight)/(MAX_HEIGHT/100));
        }

        xSemaphoreGive(UARTSemaphore);
    }
}



/**
 * Initializes the Height controller
 */
uint8_t heightControllerInit(void) {

    // Create a queue for calibrating the ground
    calibrationQueue = xQueueCreate(CALIBRATION_QUEUE_SIZE, CALIBRATION_ITEM_SIZE);

    /*
    * Create the height controller task.
    */
    if(pdTRUE !=  xTaskCreate(heightControllerTask, "heightControllerTask", HEIGHT_TASK_STACK_SIZE, NULL, tskIDLE_PRIORITY +
                   PRIORITY_HEIGHT_CONTROLLER_TASK, NULL))
    {
        return 1; // error creating task, out of memory?
    }

    return 0;
}

/*
 * Calculate height from button input
 */
uint16_t calculateNewHeight(uint16_t currentHeight, uint8_t buttonInputMessage) {

    if (buttonInputMessage == 16 && currentHeight >= (MAX_HEIGHT/10) && currentHeight <= MAX_HEIGHT) {
        return currentHeight - (MAX_HEIGHT/10);
    } else if (buttonInputMessage == 1 && currentHeight <= (MAX_HEIGHT - (MAX_HEIGHT/10))) {
        return currentHeight + (MAX_HEIGHT/10);
    } else {
        return currentHeight;
    }
}

