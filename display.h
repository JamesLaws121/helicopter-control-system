/**********************************************************
 *
 * display.h
 *
 * Houses functions related to displaying data on
 * the OLED display
 *
 *    Ben Stewart
 *
 *
 **********************************************************/

#ifndef DSIPLAY_H_
#define DISPLAY_H_

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

/**********************************************************
 * Constants
 **********************************************************/
// Systick configuration
#define SYSTICK_RATE_HZ    10
#define BUFF_SIZE 10
#define NUM_BITS 256
#define GRAVITY 9.81


void
delay_ms (uint32_t ms_delay);

void
delay_hz (uint32_t hz_delay);

void
displayUpdate (char *str1, char *str2, int32_t num, uint8_t charLine);

void
updateDisplay ();


#endif /* READACC_H_ */
