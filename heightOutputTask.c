/*
 * heightOuputTask.c
 *
 *  Created on: 7/08/2023
 *      Author: James Laws
 */


#include <stdbool.h>
#include <stdint.h>

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/rom.h"
#include "drivers/buttons.h"
#include "driverlib/pwm.h"
#include "utils/uartstdio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"

#include "FreeRTOS.h"
#include "timers.h"
#include "config.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "heightController.h"

/**
 * Variables for Initializing Motor PWM
 */
#define PWM_MOTOR_GENERATOR         PWM0_BASE
#define PWM_MAIN_GEN                PWM_GEN_3
#define PWM_MOTOR_PINOUT            PWM_OUT_7
#define PWM_PIN7_BIT                PWM_OUT_7_BIT
#define GPIO_MOTOR_BASE             GPIO_PORTC_BASE
#define GPIO_PWM_CONFIG             GPIO_PC5_M0PWM7
#define GPIO_PWM_MOTOR_PIN          GPIO_PIN_5
#define PDMS_TO_TICKS               20
#define PERCENT                     100
#define DELTA_T                      0.1
#define PWM_DIVIDER                 4

/**
 *
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


/**
* The stack size for the heightOuput task
**/
#define HEIGHT_OUTPUT_TASK_STACK_SIZE    32         // Stack size in words

/**
*The item size and queue size for the height ouput queue.
**/
#define HEIGHT_OUTPUT_ITEM_SIZE           sizeof(uint8_t)
#define HEIGHT_OUTPUT_QUEUE_SIZE          1

/**
* The queue that holds button inputs
**/
QueueHandle_t heightOutputQueue;

int32_t motorDuty;
TimerHandle_t PWMTimer;

double integratedHeightError, previousHeightError;


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
    int32_t currentHeight = getCurrentHeight();
    if (currentHeight > -1)
    {
        heightError = getDesiredtHeight() - currentHeight;    // Height Error
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

    int32_t currentHeight = getCurrentHeight();

    if (currentHeight > -1)
    {
     // Calculate the duty cycle required
    Motor_PIController(DELTA_T);
    }

    // Set the duty cycle required
    setPWM(INITIAL_PWM_FREQ, motorDuty);

}


/**
 *  Initializing Motor PWM
 *  Motor uses M0PWM7 (j4-05) as referred in Table1 TIVA MCU I/O data signals
 */
void InitializeMotorPWM(void)
{
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
        while(1);   // Whoops, we've run into an issue
    }
    // Start the timer
    if (!xTimerStart(PWMTimer, 0))
    {
        while(1);   // Whoops, we've run into an issue
    }

}



/**
* Gets the heightOuput queue
**/
QueueHandle_t getHeightOutputQueue() {
   return heightOutputQueue;
}


/**
* This task reads the heightOuputQueue and outputs the desired output
**/
static void heightOuputTask(void *pvParameters) {

    while(1)
    {
        vTaskDelay(pdMS_TO_TICKS(FREQUENCYY_HEIGHT_OUTPUT_TASK));
        /*
        *   HEIGHT output READING CODE HERE
        */

        xSemaphoreTake(UARTSemaphore, portMAX_DELAY);
        UARTprintf("\n\n Height Output Task");
        xSemaphoreGive(UARTSemaphore);
    }
}


/**
* Initializes the heightOuput task
**/
uint32_t heightOutputTaskInit(void)
{

    // Create a queue for storing height
    heightOutputQueue = xQueueCreate(HEIGHT_OUTPUT_QUEUE_SIZE, HEIGHT_OUTPUT_ITEM_SIZE);

    /*
    * Create the heightOuput task.
    */
    if(pdTRUE != xTaskCreate(heightOuputTask, "heightOuputTask", HEIGHT_OUTPUT_TASK_STACK_SIZE, NULL, tskIDLE_PRIORITY +
                   PRIORITY_HEIGHT_OUTPUT_TASK, NULL))
    {
        return(1); // error creating task, out of memory?
    }



    // Success.
    return(0);
}





