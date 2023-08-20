/*
 * heightController.c
 *
 *  Created on: 2/08/2023
 *  Authors: James Laws
 */

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
//#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/rom.h"

#include "drivers/buttons.h"
#include "drivers/uartstdio.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "config.h"
#include "buttonTask.h"
#include "heightOutputTask.h"
#include "heightController.h"
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

    uint8_t buttonInputMessage;
    uint16_t altitudeInputMessage;

    HeightStructure_t heightOutputMessage;
    heightOutputMessage.currentHeight = 0;
    heightOutputMessage.desiredHeight = 0;


    uint32_t groundVoltage = 0;

    uint8_t calibrationCountdown = 4;

    while(1)
    {
        vTaskDelay(pdMS_TO_TICKS(FREQUENCY_HEIGHT_CONTROLLER_TASK));
        xSemaphoreTake(UARTSemaphore, portMAX_DELAY);

        //UARTprintf("\n\nHeight Controller Task");


        // Read the next button input, if available on queue.
        QueueHandle_t buttonInputQueue = getButtonInputQueue();
        if(xQueueReceive(buttonInputQueue, &buttonInputMessage, 0) == pdPASS) {
            // Update height based on button buttonInput
            UARTprintf("\n BUTTON: %d", buttonInputMessage); //16 for left, 1 for right

            if (buttonInputMessage == 16 && heightOutputMessage.currentHeight > 0) {
                heightOutputMessage.desiredHeight -= 10;
            } else if (buttonInputMessage == 1) {
                heightOutputMessage.desiredHeight += 10;
            }
        }


        // Reads the altitude input and updates output accordingly
        QueueHandle_t altitudeInputQueue = getAltitudeInputQueue();

        if (xQueueReceive(altitudeInputQueue, &altitudeInputMessage, 0) == pdPASS) {
            // Calculate roter output to get to wanted altitude
            UARTprintf("\n READ FROM ALTITUDE QUEUE\n RESULT: %d",altitudeInputMessage);

            // Calibrate ground voltage
            if (calibrationCountdown != 0) {
                calibrationCountdown -= 1;
                if (calibrationCountdown == 3) {
                    UARTprintf("\n Skipping first input \n");
                } else {
                    UARTprintf("\n Calculating ground voltage. %d\n", groundVoltage);
                    groundVoltage = groundVoltage==0? altitudeInputMessage: (groundVoltage + altitudeInputMessage)/2;
                    UARTprintf("\n Calculating ground voltage. %d\n", groundVoltage);
                }


            } else {
                uint8_t calibrationMessage = 1;
                xQueueOverwrite(calibrationQueue, &calibrationMessage);

                heightOutputMessage.currentHeight = (altitudeInputMessage > groundVoltage) ? 0 : groundVoltage - altitudeInputMessage;
                // Write to output queue
                UARTprintf("\n Send data to height output\n");
                QueueHandle_t heightOutputQueue = getHeightOutputQueue();
                if(xQueueSendToBack(heightOutputQueue, &heightOutputMessage, 5) != pdPASS) {
                    UARTprintf("\nERROR: height output queue full. This should never happen.\n");
                }
            }

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
    * Create the buttons task.
    */
    if(pdTRUE !=  xTaskCreate(heightControllerTask, "heightControllerTask", HEIGHT_TASK_STACK_SIZE, NULL, tskIDLE_PRIORITY +
                   PRIORITY_HEIGHT_CONTROLLER_TASK, NULL))
    {
        return(1); // error creating task, out of memory?
    }

    return 0;
}


