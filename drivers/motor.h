/*
 * motor.h
 *
 *  Created on: 18/08/2023
 *      Author: jla201
 */

#include <stdbool.h>
#include <stdint.h>

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/rom.h"
#include "driverlib/pwm.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"

#include "drivers/buttons.h"
#include "drivers/uartstdio.h"

#include "FreeRTOS.h"
#include "timers.h"
#include "config.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "heightController.h"


#ifndef DRIVERS_MOTOR_H_
#define DRIVERS_MOTOR_H_


/**
* Function to set the Period and duty cycle of main motor
*/
void setPWM(uint32_t ui32Freq, uint32_t ui32Duty);


/*
* freeRTOS task to set the duty cylce for the main
* rotor
*/
void motorPWM(TimerHandle_t pxTimer);


/*
* Calculates the duty for the motor. Uses the AltitudeTask
* to find the current height and the desired height
*/
void Motor_PIController(double delta_t);


/**
 *  Initializing Motor PWM
 *  Motor uses M0PWM7 (j4-05) as referred in Table1 TIVA MCU I/O data signals
 */
uint8_t motorTaskInit(void);


#endif /* DRIVERS_MOTOR_H_ */
