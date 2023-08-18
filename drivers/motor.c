/*
 * heightOuputTask.c
 *
 *  Created on: 12/08/2023
 *  Edited on: 18/08/2023
 *      Editor: AJ Seville
 *      Editor: James Laws
 */


#include <stdbool.h>
#include <stdint.h>

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"


#include "driverlib/debug.h"
#include "driverlib/systick.h"
#include "driverlib/gpio.h"
#include "driverlib/rom.h"
#include "driverlib/pwm.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"

#include "drivers/pin_map.h"
#include "drivers/buttons.h"
#include "drivers/motor.h"
#include "drivers/uartstdio.h"

#include "FreeRTOS.h"
#include "timers.h"
#include "config.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "heightController.h"


/**
 * Hardware variables for Initializing Motor PWM
 */
#define PWM_MOTOR_GENERATOR         PWM0_BASE
#define PWM_MAIN_GEN                PWM_GEN_3
#define PWM_PERIPH_PWM              SYSCTL_PERIPH_PWM0
#define PWM_PERIPH_GPIO             SYSCTL_PERIPH_GPIOC
#define PWM_MOTOR_PINOUT            PWM_OUT_7
#define PWM_PIN7_BIT                PWM_OUT_7_BIT
#define GPIO_MOTOR_BASE             GPIO_PORTC_BASE
#define GPIO_PC5_M0PWM7             0x00021404    // This is already defined in pin_map.h but code compose wouldn't find it
#define GPIO_PWM_CONFIG             GPIO_PC5_M0PWM7
#define GPIO_PWM_MOTOR_PIN          GPIO_PIN_5
#define PDMS_TO_TICKS               20
#define PERCENT                     100
#define DELTA_T                      0.1
#define PWM_DIVIDER                 4

/**
 *  PWM configurations
 */
#define PWM_DUTY_FIXED              67
#define INITIAL_PWM_FREQ            250
#define MAX_PWM                     85
#define MIN_PWM                     15
#define MOTOR_CONSTANT              15
#define MOTOR_KP                    0.45
#define MOTOR_KI                    0.0075


#define MAX_ERROR                   20
#define MIN_ERROR                  -20



int32_t motorDuty;
TimerHandle_t PWMTimer;

double integratedHeightError, previousHeightError;



/**
* Function to set the Period and duty cycle of main motor
*/
void setPWM(uint32_t ui32Freq, uint32_t ui32Duty)
{
    // Calculate the PWM period corresponding to the freq.
    uint32_t ui32Period = SysCtlClockGet() / PWM_DIVIDER / ui32Freq;

    PWMGenPeriodSet(PWM_MOTOR_GENERATOR, PWM_MAIN_GEN, ui32Period);
    PWMPulseWidthSet(PWM_MOTOR_GENERATOR, PWM_MOTOR_PINOUT,
                     ui32Period * ui32Duty / PERCENT);
}


/*
* freeRTOS task to set the duty cylce for the main
* rotor
*/
void motorPWM(TimerHandle_t pxTimer)
{

    int32_t currentHeight = 0;

    if (currentHeight > -1)
    {
        // Calculate the duty cycle required
        Motor_PIController(DELTA_T);
    }

    // Set the duty cycle required
    setPWM(INITIAL_PWM_FREQ, motorDuty);
}


/*
* Calculates the duty for the motor. Uses the AltitudeTask
* to find the current height and the desired height
*/
void Motor_PIController(double delta_t)
{
    double heightError;                                    // The altitude error

    // The PI controller gains for the main rotor
    double kpHeight = MOTOR_KP;                         // Proportional Gain
    double kiHeight = MOTOR_KI;                         // Integral Gains

    // Error calc for height (Need to current height from Altitude task?)
    int32_t currentHeight = 0;
    if (currentHeight > -1)
    {
        heightError = 0 - currentHeight;    // Height Error
    }

    // Need to cap the errors to avoid overshooting the target (controller patch)
    if (heightError >= MAX_ERROR)
    {
        heightError = MAX_ERROR;
    }

    else if (heightError <= MIN_ERROR)
    {
        heightError = MIN_ERROR;
    }

    integratedHeightError += heightError * delta_t;            // Integral Height Error

    // Calculation of the main rotor's duty cycle
    int32_t mainDuty = MOTOR_CONSTANT + kpHeight * heightError + kiHeight * integratedHeightError;


    // Specified limits given from the lecture
    if (mainDuty > MAX_PWM)
    {
        motorDuty = MAX_PWM;                       // Set the duty cycle ceiling
    }

    else if (mainDuty < MIN_PWM)
    {
        motorDuty = MIN_PWM;                       // Set the duty cycle floor
    }

    else
    {
        motorDuty = mainDuty;
    }

        // Recored previous height error for the integral error
    previousHeightError = heightError;
}




/**
 *  Initializing Motor PWM
 *  Motor uses M0PWM7 (j4-05) as referred in Table1 TIVA MCU I/O data signals
 */
uint8_t motorTaskInit(void)
{

    SysCtlPeripheralEnable(PWM_PERIPH_PWM);
    SysCtlPeripheralEnable(PWM_PERIPH_GPIO);

    GPIOPinConfigure(GPIO_PWM_CONFIG);
    GPIOPinTypePWM(GPIO_MOTOR_BASE, GPIO_PWM_MOTOR_PIN);
    PWMGenConfigure(PWM_MOTOR_GENERATOR, PWM_MOTOR_GENERATOR, PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);

    // Initialize parameters for PWM
    setPWM(INITIAL_PWM_FREQ, PWM_DUTY_FIXED);
    PWMGenEnable(PWM_MOTOR_GENERATOR, PWM_MOTOR_GENERATOR);

    SysCtlPWMClockSet(SYSCTL_PWMDIV_4);

    // Need to make timers to trigger rotor controller
    PWMTimer = xTimerCreate("MotorTimer", pdMS_TO_TICKS(PDMS_TO_TICKS), pdTRUE, (void*) 5, motorPWM);

    if (PWMTimer == NULL)
    {
        return 1;
    }

    // Start the timer
    if (!xTimerStart(PWMTimer, 0))
    {
        return 1;
    }

    return 0;

}

