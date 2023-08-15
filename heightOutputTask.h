/*
 * heightOuputTask.h
 * James Laws
 *  Last modified: 7/08/2023 
 */

#ifndef HEIGHTOUTPUT_H_
#define HEIGHTOUTPUT_H_

#include "timers.h"

/**
* Initializes the height output task
**/
uint32_t heightOutputTaskInit (void);

/**
* Gets the heightOuput queue
**/
QueueHandle_t getHeightOutputQueue();

/*
* Initialize the PWM output on Tiva Board
*/
void InitializeMotorPWM(void);

/*
*Calculates the duty for the motor. Uses the AltitudeTask
* to find the current height and the desired height
*/
void Motor_PIController(double delta_t);

/*
* Function to set the Period and duty cycle of motor
*/
void setPWM(uint32_t ui32Freq, uint32_t ui32Duty);


/*
* freeRTOS task to set the duty cylce for the main
* rotor
*/
void motorPWM(TimerHandle_t pxTimer);


#endif /* HEIGHTOUTPUT_H_ */
