
/*
* config.h - config for the various FreeRTOS tasks.
*/


#ifndef CONFIG_H_
#define CONFIG_H_


// The priorities of the various tasks.

#define PRIORITY_BUTTONS_TASK               1
#define PRIORITY_HEIGHT_CONTROLLER_TASK     1
#define PRIORITY_ALTITUDE_TASK              1
#define PRIORITY_HEIGHT_OUTPUT_TASK         1

// The frequencies of the various tasks.

#define FREQUENCY_BUTTONS_TASK              500
#define FREQUENCY_HEIGHT_CONTROLLER_TASK    600
#define FREQUENCY_ALTITUDE_TASK             700
#define FREQUENCYY_HEIGHT_OUTPUT_TASK       800


#endif /* CONFIG_H_ */
