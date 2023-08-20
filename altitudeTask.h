/*
 * altitudeTask.h
 *
 *  Created on: 8/08/2023
 *      Authors: James Laws, Tom Clifton
 */


#include "FreeRTOS.h"
#include "queue.h"

#ifndef ALTITUDETASK_H_
#define ALTITUDETASK_H_

/*
* Initializes the altitude task
*/
extern uint8_t altitudeTaskInit(void);

/**
* Gets the altitude input queue
**/
extern QueueHandle_t getAltitudeInputQueue(void);

#endif // ALTITUDETASK_H_
