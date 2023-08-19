/*
 * heightController.h
 * James Laws
 * Last modified: 2/08/2023 
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


uint16_t calculateNewHeight(uint16_t currentHeight, uint8_t buttonInputMessage);
void calculateNewHeightTest(uint16_t currentHeight);


#endif /* HEIGHTCONTROLLER_H_ */
