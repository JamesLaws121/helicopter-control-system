/*
 * heightOuputTask.h
 *
 *  Created on: 18/08/2023
 *  Authors: AJ Seville, James Laaws , Thomas Clifton
 */


#include <stdint.h>
#include "FreeRTOS.h"
#include "queue.h"
#include "heightController.h"


#define INITIAL_PWM_FREQ    250


#ifndef DRIVERS_MOTOR_H_
#define DRIVERS_MOTOR_H_


/*
 * Struct for storing motor duty and the integrated heigh error
 */
typedef struct PWMStructure {
    double integratedHeightError;
    int32_t mainDuty;
} PWMStructure_t;



/**
* Function to set the Period and duty cycle of main motor
*/
void setPWM(uint32_t ui32Freq, uint32_t ui32Duty);


/*
* Calculates the duty for the motor.
* Takes current height and the desired height
*/
PWMStructure_t calculateMotorDuty(HeightStructure_t heightInput, double integratedHeightError);


/**
 *  Initializing Motor PWM
 *  Motor uses M0PWM7 (j4-05) as referred in Table1 TIVA MCU I/O data signals
 */
uint8_t motorInit(void);


#endif /* DRIVERS_MOTOR_H_ */
