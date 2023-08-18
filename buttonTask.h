
/*
* buttonTask.h - prototypes for the button task
*/

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
