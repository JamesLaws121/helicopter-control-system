/*
 * heightOuputTask.c
 *
 *  Created on: 7/08/2023
 *  Authors: James Laws, AJ Seville
 */


#include <stdbool.h>
#include <stdint.h>

#include "FreeRTOS.h"
#include "timers.h"
#include "task.h"
#include "queue.h"
#include "config.h"

#include "drivers/uartstdio.h"
#include "drivers/motor.h"

#include "heightController.h"
#include "heightOutputTask.h"




/**
* The stack size for the heightOuput task
**/
#define HEIGHT_OUTPUT_TASK_STACK_SIZE    256         // Stack size in words

/**
*The item size and queue size for the height output queue.
**/
#define HEIGHT_OUTPUT_ITEM_SIZE           sizeof(HeightStructure_t)
#define HEIGHT_OUTPUT_QUEUE_SIZE          1

/**
* The queue that holds button inputs
**/
QueueHandle_t heightOutputQueue;



/**
* Gets the heightOuput queue
**/
QueueHandle_t getHeightOutputQueue(void) {
   return heightOutputQueue;
}


/**
* This task reads the heightOuputQueue and outputs the desired output
**/
static void heightOuputTask(void *pvParameters) {
    uint8_t calibration_state = 0;


    HeightStructure_t heightOutput;
    heightOutput.currentHeight = 0;
    heightOutput.desiredHeight = 0;


    PWMStructure_t heightData;
    heightData.integratedHeightError = 0;
    heightData.mainDuty = 0;


    while (calibration_state == 0) {
        vTaskDelay(pdMS_TO_TICKS(CALIBRATION_FREQUENCY + FREQUENCYY_HEIGHT_OUTPUT_TASK));
        QueueHandle_t calibrationQueue = getCalibrationQueue();
        xQueuePeek( calibrationQueue, &calibration_state, 0 );
    }

    while(1)
    {

        vTaskDelay(pdMS_TO_TICKS(FREQUENCYY_HEIGHT_OUTPUT_TASK));

        xQueueReceive(heightOutputQueue, &heightOutput, 0);

        // Calculate the duty cycle required
        heightData = calculateMotorDuty(heightOutput, heightData.integratedHeightError);

        // Set the duty cycle required
        setPWM(INITIAL_PWM_FREQ, heightData.mainDuty);

    }
}


/**
* Initializes the heightOuput task
**/
uint8_t heightOutputTaskInit(void)
{

    /*
    * Initializes motor
    */
    if(motorInit() != 0)
    {
        return 1;
    }

    // Create a queue for storing height
    heightOutputQueue = xQueueCreate(HEIGHT_OUTPUT_QUEUE_SIZE, HEIGHT_OUTPUT_ITEM_SIZE);

    /*
    * Create the heightOuput task.
    */
    if(pdTRUE != xTaskCreate(heightOuputTask, "heightOuputTask", HEIGHT_OUTPUT_TASK_STACK_SIZE, NULL, tskIDLE_PRIORITY +
                   PRIORITY_HEIGHT_OUTPUT_TASK, NULL))
    {
        return 1; // error creating task, out of memory?
    }

    // Success.
    return(0);
}





