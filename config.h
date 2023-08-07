
/*
* config.h - config for the various FreeRTOS tasks.
*/


#ifndef CONFIG_H_
#define CONFIG_H_


// The priorities of the various tasks.

#define PRIORITY_BUTTON_TASK                1
#define PRIORITY_HEIGHT_CONTROLLER_TASK     1
#define PRIORITY_ALTITUDE_TASK              1
#define PRIORITY_HEIGHT_OUTPUT_TASK         1

// The frequencies of the various tasks.

#define FREQUENCY_BUTTON_TASK              5000
#define FREQUENCY_HEIGHT_CONTROLLER_TASK    6000
#define FREQUENCY_ALTITUDE_TASK             7000
#define FREQUENCYY_HEIGHT_OUTPUT_TASK       8000


#endif /* CONFIG_H_ */
