#ifndef UI_STATE_H
#define UI_STATE_H

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#define pageMAX_PAGES 2                   // Max num of pages on monitor
#define gatekeeperTASK_DATA_MAX_LENGHT 32 // Max lenght of char* that was refered into queue

extern volatile int current_page_g;
extern TaskHandle_t page_task_handle;

extern QueueHandle_t gatekeeper_queueHandle;

typedef struct
{
    int task_id;
    char data[gatekeeperTASK_DATA_MAX_LENGHT];
} GatekeeperQueueData_t;

#endif