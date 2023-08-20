/*
 * HeightController.c
 *
 *  Created on: 2/08/2023
 *      Author: James Laws, Ben
 */

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
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

#define MAX_HEIGHT 1300

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


    int8_t groundVoltage = -1;

    while(1)
    {
        vTaskDelay(pdMS_TO_TICKS(FREQUENCY_HEIGHT_CONTROLLER_TASK));
        xSemaphoreTake(UARTSemaphore, portMAX_DELAY);

        UARTprintf("\n\nHeight Controller Task");


        // Read the next button input, if available on queue.
        QueueHandle_t buttonInputQueue = getButtonInputQueue();
        if(xQueueReceive(buttonInputQueue, &buttonInputMessage, 0) == pdPASS) {
            // Update height based on button buttonInput
            UARTprintf("\n BUTTON: %d", buttonInputMessage); //16 for left, 1 for right

            heightOutputMessage.desiredHeight = calculateNewHeight(heightOutputMessage.currentHeight, buttonInputMessage);
        }

        // Reads the altitude input and updates output accordingly
        QueueHandle_t altitudeInputQueue = getAltitudeInputQueue();

        if (xQueueReceive(altitudeInputQueue, &altitudeInputMessage, 0) == pdPASS) {
            // Calculate roter output to get to wanted altitude
            UARTprintf("\n READ FROM ALTITUDE QUEUE\n RESULT: %d",altitudeInputMessage);
            heightOutputMessage.currentHeight = altitudeInputMessage/10;

            // Calibrate ground voltage
            if (groundVoltage == -1) {
                UARTprintf("\n Setting ground voltage.\n");
                groundVoltage = altitudeInputMessage;

                uint8_t calibrationMessage = 1;
                if(xQueueSendToBack(calibrationQueue, &calibrationMessage , 5) != pdPASS) {
                    UARTprintf("\nERROR: calibration queue full. This should never happen.\n");
                }
            } else {
                // Write to output queue
                UARTprintf("\n Send data to height output\n");
                QueueHandle_t heightOutputQueue = getHeightOutputQueue();
                if(xQueueOverwrite(heightOutputQueue, &heightOutputMessage) != pdPASS) {
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

int calculateNewHeight(int currentHeight, uint8_t buttonInputMessage) {
    /*
     * Calculate height from button input
     */
    if (buttonInputMessage == 16 && currentHeight >= MAX_HEIGHT/10 && currentHeight <= MAX_HEIGHT) {
        return currentHeight - MAX_HEIGHT/10;
    } else if (buttonInputMessage == 1 && currentHeight >= 0 && currentHeight <= (MAX_HEIGHT - MAX_HEIGHT/10)) {
        return currentHeight + MAX_HEIGHT/10;
    } else {
        return currentHeight;
    }
}

void calculateNewHeightTest(uint16_t currentHeight) {
    /*
     * Black box testing new height calculation
     */

    //16 for left, 1 for right
    UARTprintf("\n\n--------------\n");
    UARTprintf("Height calculation tests\n\n");

    //Wrong button number
    UARTprintf("Test 1 result, %s\n", calculateNewHeight(5, 10) == 5 ? "PASS" : "FAIL"); //Won't increase height

    //Valid height numbers
    UARTprintf("Test 2 result, %s\n", calculateNewHeight(5, 1) != 5 ? "PASS" : "FAIL"); //Will
    UARTprintf("Test 3 result, %s\n", calculateNewHeight(502, 1) != 502 ? "PASS" : "FAIL"); //Will
    UARTprintf("Test 4 result, %s\n", calculateNewHeight(1050, 16) != 1050 ? "PASS" : "FAIL"); //Will

    //Invalid height numbers
    UARTprintf("Test 5 result, %s\n", calculateNewHeight(5, 16) == 5 ? "PASS" : "FAIL");
    UARTprintf("Test 6 result, %s\n", calculateNewHeight(1290, 1) == 1290 ? "PASS" : "FAIL");
    UARTprintf("Test 7 result, %s\n", calculateNewHeight(-500, 16) == -500 ? "PASS" : "FAIL");
    UARTprintf("Test 8 result, %s\n", calculateNewHeight(-5, 1) == -5 ? "PASS" : "FAIL");
}
