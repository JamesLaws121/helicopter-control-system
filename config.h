
/*
* config.h - config for the various FreeRTOS tasks.
*/

#include "FreeRTOS.h"
#include "semphr.h"


#ifndef CONFIG_H_
#define CONFIG_H_


// The priorities of the various tasks.

#define PRIORITY_BUTTON_TASK                1
#define PRIORITY_HEIGHT_CONTROLLER_TASK     2
#define PRIORITY_ALTITUDE_TASK              3
#define PRIORITY_HEIGHT_OUTPUT_TASK         4


/*  The frequencies of the various tasks.
 *  Measured in milliseconds
 *  These are purposely prime numbers
 */

#define FREQUENCY_BUTTON_TASK               52
#define FREQUENCY_HEIGHT_CONTROLLER_TASK    173
#define FREQUENCY_ALTITUDE_TASK             213
#define FREQUENCYY_HEIGHT_OUTPUT_TASK       727
#define CALIBRATION_FREQUENCY               3027


// The UART semaphore
extern SemaphoreHandle_t UARTSemaphore;



#endif /* CONFIG_H_ */
