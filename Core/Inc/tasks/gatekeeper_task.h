#include "queue.h"

#define __GATEKEEPER_TASK_H
#ifdef __GATEKEEPER_TASK_H
#define gatekeeper_watchdogMIN_TIME 80
#define gatekeeper_watchdogMAX_TIME 120
#define gatekeeperQUEUE_MAX_WAIT_MS 100 // Max blocking time for task (needed for watchdog)
#define gatekeeperQUEUE_LENGHT 10       // Max lenght of the queue
#define gatekeeperMAX_TASK 8            // Max number of tasks that gatekeeper can store
#define gatekeeperCHAR_MAX_LENGHT 32    // Max lenght of char* that was refered into queue

typedef struct
{
    int taskId;
    char data[gatekeeperCHAR_MAX_LENGHT];
} gatekeeperQueueData_t;

typedef struct
{
    QueueHandle_t queueHandler;
    const char taskNames[gatekeeperMAX_TASK][gatekeeperCHAR_MAX_LENGHT];
} gatekeeperTask_Config_t;

void gatekeeperTask(void *pvParameters);

#endif