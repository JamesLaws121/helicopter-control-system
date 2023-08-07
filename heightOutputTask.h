/*
 * heightOuputTask.h
 *
 *  Created on: 7/08/2023
 *      Author: James Laws
 */

#ifndef HEIGHTOUTPUT_H_
#define HEIGHTOUTPUT_H_


/**
* Initializes the height output task
**/
extern uint32_t heightOuputTaskInit (void);

/**
* Gets the heightOuput queue
**/
extern QueueHandle_t getHeightOutputQueue();


#endif /* HEIGHTOUTPUT_H_ */
