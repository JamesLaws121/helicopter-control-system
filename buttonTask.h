
/*
* buttonTask.h - prototypes for the button task
*/

#include "queue.h"
#include <stdint.h>

#ifndef BUTTTONTASK_H_
#define BUTTTONTASK_H_

/*
* Initializes the buttons task
*/
extern uint32_t buttonTaskInit(void);

/**
* Gets the button input queue
**/
extern QueueHandle_t getButtonInputQueue();

#endif // BUTTTONTASK_H_
