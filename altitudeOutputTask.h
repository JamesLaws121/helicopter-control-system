/*
 * altitudeOuputTask.h
 *
 *  Created on: 7/08/2023
 *  Authors: James Laws, AJ Seville
 */

#include "FreeRTOS.h"
#include "queue.h"

#ifndef ALTITUDEOUTPUT_H_
#define ALTITUDEOUTPUT_H_


/**
* Initializes the height output task
**/
uint8_t altitudeOutputTaskInit(void);

/**
* Gets the heightOuput queue
**/
QueueHandle_t getAltitudeOutputQueue(void);


#endif /* ALTITUDEOUTPUT_H_ */
