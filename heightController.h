/*
 * heightController.h
 * James Laws
 * Last modified: 2/08/2023 
 */

#include "stdint.h"

#ifndef HEIGHTCONTROLLER_H_
#define HEIGHTCONTROLLER_H_

uint32_t heightControllerInit (void);


QueueHandle_t getCalibrationQueue();


/*
 * Struct for storing current and desired helicopter height
 */
typedef struct HeightStructure {
    uint16_t currentHeight;
    uint8_t desiredHeight;
} HeightStructure_t;




#endif /* HEIGHTCONTROLLER_H_ */
