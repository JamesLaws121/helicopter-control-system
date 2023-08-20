/*
 * analogueConverter.h
 *
 *  Created on: 8/08/2023
 *      Authors: P.J. Bones UCECE, Tom Clifton, James Laws
 */

#include <stdint.h>

#ifndef ANALOGUECONVERTER_H_
#define ANALOGUECONVERTER_H_

/*
 * The handler for the analogue to digital conversion interrupt.
*/
void ADCIntHandler(void);

/*
* Initializes the ADC peripheral
*/
uint8_t initADC(void);

/*
* Calculates and return the rounded mean of the buffer contents
*/
uint16_t getBufferMean(void);

#endif /* ANALOGUECONVERTER_H_ */
