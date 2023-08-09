/*
 * analogueConverter.c
 * P.J. Bones UCECE, James Laws
 * Last modified: 8/Aug/2023
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
void initADC (void);

/*
* Calculates and return the rounded mean of the buffer contents
*/
uint16_t getBufferMean();

#endif /* ANALOGUECONVERTER_H_ */
