/*
 * analogueConverter.c
 * P.J. Bones   UCECE, J. Laws
 * Last modified: 8/Aug/2023
*/


#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/adc.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/debug.h"

#include "drivers/ustdlib.h"
#include "types/circBufT.h"

#include "analogueConverter.h"

// Buffer size
#define BUF_SIZE 10

// Buffer for storing conversion results
static circBuf_t adcBuffer;


/*
 * The handler for the analogue to digital conversion interrupt.
*/
void ADCIntHandler(void)
{
	uint32_t ulValue;

	// Get the single sample from ADC0.
	ADCSequenceDataGet(ADC0_BASE, 3, &ulValue);

	// Place it in the circular buffer (advancing write index)
	writeCircBuf (&adcBuffer, ulValue);

    // Clean up, clearing the interrupt
    ADCIntClear(ADC0_BASE, 3);
                       
}

/*
* Initializes the ADC peripheral
*/
uint8_t initADC(void)
{
    // initialize the altitude circular buffer
    initCircBuf(&adcBuffer, BUF_SIZE);

    // The ADC0 peripheral must be enabled for configuration and use.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);

    SysCtlDelay(3);
    ADCReferenceSet(ADC0_BASE, ADC_REF_INT);
    
    // Enable sample sequence 3 with a processor signal trigger.  Sequence 3
    // will do a single sample when the processor sends a signal to start the
    // conversion.
    ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);
  

    ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH9 | ADC_CTL_IE |
                             ADC_CTL_END);
                             

    // Since sample sequence 3 is now configured, it must be enabled.
    ADCSequenceEnable(ADC0_BASE, 3);

    // Register the interrupt handler
    ADCIntRegister (ADC0_BASE, 3, ADCIntHandler);

    // Enable interrupts for ADC0 sequence 3 (clears any outstanding interrupts)
    ADCIntEnable(ADC0_BASE, 3);

    return 1;
}

/*
* Calculates and return the rounded mean of the buffer contents
*/
uint16_t getBufferMean(void){
    uint32_t sum = 0;
    uint8_t i;
    for (i = 0; i < BUF_SIZE; i++)
        sum = sum + readCircBuf(&adcBuffer);
        

    return ((2 * sum + BUF_SIZE) / 2 / BUF_SIZE);
}

