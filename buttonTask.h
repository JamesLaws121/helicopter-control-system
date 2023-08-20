/*
 * buttonTask.h
 *
 *  Created on: 7/08/2023
 *      Authors: Benjamin Stewart, James Laws
 */

#include "queue.h"

#ifndef BUTTTONTASK_H_
#define BUTTTONTASK_H_

/*
* Initializes the buttons task
*/
uint8_t buttonTaskInit(void);

/**
* Gets the button input queue
**/
QueueHandle_t getButtonInputQueue(void);

#endif // BUTTTONTASK_H_
