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
void heightOutputInit (void);

/**
* Gets the heightOuput queue
**/
extern xQueueHandle heightOuputQueue();


#endif /* HEIGHTOUTPUT_H_ */
