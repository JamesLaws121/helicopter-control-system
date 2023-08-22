/*
 * heightControllerTests.c
 *
 *  Created on: 22/08/2023
 *      Author: Benjamin Stewart
 */

#include <stdint.h>

/*
 * Black box testing new height calculation
 */
void calculateNewHeightTest(uint16_t currentHeight) {
    //16 for left, 1 for right
    UARTprintf("\n\n--------------\n");
    UARTprintf("Height calculation tests\n\n");

    //Wrong button number
    UARTprintf("Test 1 result, %s\n", calculateNewHeight(5, 10) == 5 ? "PASS" : "FAIL"); //Won't increase height

    //Valid height numbers
    UARTprintf("Test 2 result, %s\n", calculateNewHeight(5, 1) != 5 ? "PASS" : "FAIL"); //Will
    UARTprintf("Test 3 result, %s\n", calculateNewHeight(502, 1) != 502 ? "PASS" : "FAIL"); //Will
    UARTprintf("Test 4 result, %s\n", calculateNewHeight(1050, 16) != 1050 ? "PASS" : "FAIL"); //Will

    //Invalid height numbers
    UARTprintf("Test 5 result, %s\n", calculateNewHeight(5, 16) == 5 ? "PASS" : "FAIL");
    UARTprintf("Test 6 result, %s\n", calculateNewHeight(1290, 1) == 1290 ? "PASS" : "FAIL");

    //Exact test
    UARTprintf("Test 7 result, %s\n", calculateNewHeight(0, 1) == 130 ? "PASS" : "FAIL"); //Will
    UARTprintf("Test 8 result, %s\n", calculateNewHeight(130, 1) == 260 ? "PASS" : "FAIL"); //Will
}


