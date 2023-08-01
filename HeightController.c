/*
 * HeightController.c
 *
 *  Created on: 2/08/2023
 *      Author: James Laws
 */

#include <FreeRTOS.h>
#include <stdio.h>
#include <task.h>

static void triggerHeightCollection();

static void readHeightInput();


/**
 * Initializes the Height controller
 */
void initHeightController() {


    if (pdTRUE != xTaskCreate(triggerHeightCollection, "Trigger Task", 32, (void *)1, 4, NULL)) {
        while (1) ; // error creating task, out of memory?
    }

    if (pdTRUE != xTaskCreate(readHeightInput, "Read Task", 32, (void *)1, 4, NULL)) {
        while (1) ; // error creating task, out of memory?
    }

}



static void readHeightInput(void *pvParameters) {

    while(1)
    {
        //uint32_t x = getAltitudeData();
        printf("Get Altitude Data");

        // Runs every 5000 millisecond
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}

/**
 * Triggers the collections of altitude data
 */
static void triggerHeightCollection() {

    while(1)
    {
       // uint32_t x = triggerAltitudeADC();
        printf("Trigger Altitude ADC");

        // Runs every 100 millisecond
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void vAssertCalled( const char * pcFile, unsigned long ulLine ) {
    (void) pcFile;
    (void) ulLine;
    while (1) ;
}

