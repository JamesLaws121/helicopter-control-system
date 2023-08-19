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

#define INITIAL_PWM_FREQ    250


#ifndef DRIVERS_MOTOR_H_
#define DRIVERS_MOTOR_H_


TimerHandle_t PWMTimer;


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
* Calculates the duty for the motor. Uses the AltitudeTask
* to find the current height and the desired height
*/
PWMStructure_t calculateMotorDuty(HeightStructure_t heightInput, double integratedHeightError);


/**
 *  Initializing Motor PWM
 *  Motor uses M0PWM7 (j4-05) as referred in Table1 TIVA MCU I/O data signals
 */
uint8_t motorInit(void);


#endif /* DRIVERS_MOTOR_H_ */
