/*
 * altitudeOuputTask.c
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

#include "altitudeOutputTask.h"
#include "heightController.h"




/**
* The stack size for the altitudeOutput task
**/
#define ALTITUDE_OUTPUT_TASK_STACK_SIZE    256         // Stack size in words

/**
*The item size and queue size for the altitude output queue.
**/
#define ALTITUDE_OUTPUT_ITEM_SIZE       sizeof(HeightStructure_t)
#define ALTITUDE_OUTPUT_QUEUE_SIZE      1

/**
* The queue that holds altitude outputs
**/
QueueHandle_t altitudeOutputQueue;



/**
* Gets the heightOuput queue
**/
QueueHandle_t getAltitudeOutputQueue(void) {
   return altitudeOutputQueue;
}


/**
* This task reads the altitudeOuputQueue and outputs the desired output
**/
static void altitudeOutputTask(void *pvParameters) {
    uint8_t calibration_state = 0;


    HeightStructure_t altitudeOutput;
    altitudeOutput.currentHeight = 0;
    altitudeOutput.desiredHeight = 0;


    PWMStructure_t heightData;
    heightData.integratedHeightError = 0;
    heightData.mainDuty = 0;


    while (calibration_state == 0) {
        vTaskDelay(pdMS_TO_TICKS(CALIBRATION_FREQUENCY + FREQUENCYY_ALTITUDE_OUTPUT_TASK));
        QueueHandle_t calibrationQueue = getCalibrationQueue();
        xQueuePeek( calibrationQueue, &calibration_state, 0 );
    }

    while(1)
    {

        vTaskDelay(pdMS_TO_TICKS(FREQUENCYY_ALTITUDE_OUTPUT_TASK));

        if (xQueueReceive(altitudeOutputQueue, &altitudeOutput, 0) == pdPASS) {
            // Calculate the duty cycle required
            heightData = calculateMotorDuty(altitudeOutput, heightData.integratedHeightError);

            // Set the duty cycle required
            setPWM(INITIAL_PWM_FREQ, heightData.mainDuty);
        }
    }
}


/**
* Initializes the altitudeOutput task
**/
uint8_t altitudeOutputTaskInit(void)
{

    /*
    * Initializes the pwm motor
    */
    if(motorInit() != 0)
    {
        return 1;
    }

    // Create a queue for storing altitude
    altitudeOutputQueue = xQueueCreate(ALTITUDE_OUTPUT_QUEUE_SIZE, ALTITUDE_OUTPUT_ITEM_SIZE);

    /*
    * Create the altitudeOutput task.
    */
    if(pdTRUE != xTaskCreate(altitudeOutputTask, "altitudeOuputTask", ALTITUDE_OUTPUT_TASK_STACK_SIZE, NULL, tskIDLE_PRIORITY +
                   PRIORITY_ALTITUDE_OUTPUT_TASK, NULL))
    {
        return 1; // error creating task, out of memory?
    }

    // Success.
    return 0;
}





