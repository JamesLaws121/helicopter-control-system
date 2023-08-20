
/*
* altitudeTask.h - prototypes for the altitude task
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
extern QueueHandle_t getAltitudeInputQueue();

#endif // ALTITUDETASK_H_
