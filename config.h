
/*
*  config.h - config for the various FreeRTOS tasks.
*
 *  Created on: 7/08/2023
 *      Authors: James Laws
 */

#include "FreeRTOS.h"
#include "semphr.h"


#ifndef CONFIG_H_
#define CONFIG_H_


// The priorities of the various tasks.

#define PRIORITY_BUTTON_TASK                4
#define PRIORITY_HEIGHT_CONTROLLER_TASK     3
#define PRIORITY_ALTITUDE_TASK              2
#define PRIORITY_ALTITUDE_OUTPUT_TASK       1


/*  The frequencies of the various tasks.
 *  Measured in milliseconds
 */

#define FREQUENCY_BUTTON_TASK               23
#define FREQUENCY_HEIGHT_CONTROLLER_TASK    29
#define FREQUENCY_ALTITUDE_TASK             37
#define FREQUENCYY_ALTITUDE_OUTPUT_TASK       43
#define CALIBRATION_FREQUENCY               241


// The UART semaphore
extern SemaphoreHandle_t UARTSemaphore;



#endif /* CONFIG_H_ */
