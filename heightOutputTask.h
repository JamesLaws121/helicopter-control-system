/*
 * heightOuputTask.h
 * James Laws
 *  Last modified: 7/08/2023 
 */

#ifndef HEIGHTOUTPUT_H_
#define HEIGHTOUTPUT_H_


/**
* Initializes the height output task
**/
uint32_t heightOuputTaskInit (void);

/**
* Gets the heightOuput queue
**/
QueueHandle_t getHeightOutputQueue();


#endif /* HEIGHTOUTPUT_H_ */
