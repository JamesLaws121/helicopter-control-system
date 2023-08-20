/*
 * heightOuputTask.h
 *
 *  Created on: 7/08/2023
 *  Edited on: 12/08/2023
 *      Author: James Laws
 *      Editor: AJ Seville
 */

#ifndef HEIGHTOUTPUT_H_
#define HEIGHTOUTPUT_H_


/**
* Initializes the height output task
**/
uint8_t heightOutputTaskInit (void);

/**
* Gets the heightOuput queue
**/
QueueHandle_t getHeightOutputQueue(void);


#endif /* HEIGHTOUTPUT_H_ */
