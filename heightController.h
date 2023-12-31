/*
 * heightController.h
 *
 *  Created on: 2/08/2023
 *  Authors: James Laws
 */

#include "stdint.h"
#include "FreeRTOS.h"
#include "queue.h"

#ifndef HEIGHTCONTROLLER_H_
#define HEIGHTCONTROLLER_H_


/**
 * Initializes the Height controller
 */
uint8_t heightControllerInit(void);


/**
* Gets the calibration state queue
**/
QueueHandle_t getCalibrationQueue(void);


/*
 * Struct for storing current and desired helicopter height
 */
typedef struct HeightStructure {
    uint16_t currentHeight;
    uint16_t desiredHeight;
} HeightStructure_t;


/*
 * Calculate height from button input
 */
uint16_t calculateNewHeight(uint16_t currentHeight, uint8_t buttonInputMessage);

/*
 * Black box testing new height calculation
 */
void calculateNewHeightTest(uint16_t currentHeight);


#endif /* HEIGHTCONTROLLER_H_ */
