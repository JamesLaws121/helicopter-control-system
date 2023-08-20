/*
 * heightController.h
 *
 *  Created on: 2/08/2023
 *  Authors: James Laws
 */

#include "stdint.h"

#ifndef HEIGHTCONTROLLER_H_
#define HEIGHTCONTROLLER_H_


uint8_t heightControllerInit(void);


QueueHandle_t getCalibrationQueue(void);


/*
 * Struct for storing current and desired helicopter height
 */
typedef struct HeightStructure {
    uint16_t currentHeight;
    uint16_t desiredHeight;
} HeightStructure_t;




#endif /* HEIGHTCONTROLLER_H_ */
