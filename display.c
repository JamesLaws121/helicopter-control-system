/**********************************************************
 *
 * display.c
 *
 * Houses functions related to displaying data on
 * the OLED display
 *
 *    Ben Stewart
 *
 *
 **********************************************************/

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_i2c.h"
#include "driverlib/pin_map.h" //Needed for pin configure
#include "driverlib/systick.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/i2c.h"
#include "driverlib/interrupt.h"
#include "inc/hw_ints.h"
#include "OrbitOLED/OrbitOLEDInterface.h"
#include "utils/ustdlib.h"
#include <math.h>


/**********************************************************
 * Constants
 **********************************************************/
// Systick configuration
#define SYSTICK_RATE_HZ    10
#define BUFF_SIZE 10
#define NUM_BITS 256
#define GRAVITY 9.81

//*****************************************************************************
// Function to display a changing message on the display.
// The display has 4 rows of 16 characters, with 0, 0 at top left.
//*****************************************************************************
void
displayUpdate (char *str1, char *str2, int32_t num, uint8_t charLine)
{
    char text_buffer[17];           //Display fits 16 characters wide.

    // "Undraw" the previous contents of the line to be updated.
    OLEDStringDraw ("                ", 0, charLine);
    // Form a new string for the line.  The maximum width specified for the
    //  number field ensures it is displayed right justified.
    usnprintf(text_buffer, sizeof(text_buffer), "%s %s %3d", str1, str2, num);
    // Update line on display.
    OLEDStringDraw (text_buffer, 0, charLine);
}

/**
 * Delays for specific number of milliseconds
 */
void delay_ms (uint32_t ms_delay)
{
    SysCtlDelay(ms_delay * (SysCtlClockGet() / 3 / 1000));
}

/**
 * Runs display at specific frequency
 */
void delay_hz (uint32_t hz_delay)
{
    delay_ms(1000/hz_delay);
}

/**
 * Calculates value to display for goal value
 */


/*
 * Displays different information based on the global state variable
 */
void updateDisplay ()
{

    OLEDStringDraw ("    Set goal    ", 0, 0);
    OLEDStringDraw ("                ", 0, 1);
    OLEDStringDraw ("      Hi        ", 0, 2);
    OLEDStringDraw ("                ", 0, 3);

}
