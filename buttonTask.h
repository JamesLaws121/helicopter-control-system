
/*
* buttonTask.h - prototypes for the button task
*/

#ifndef BUTTTONTASK_H_
#define BUTTTONTASK_H_

/*
* Initializes the buttons task
*/
extern uint32_t buttonTaskInit(void);

/**
* Gets the button input queue
**/
extern xQueueHandle getButtonInputQueue();

#endif // BUTTTONTASK_H_
