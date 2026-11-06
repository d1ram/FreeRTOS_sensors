// Gatekeeper task for writing into OLED-display
#define __GATEKEEPER_TASK_H
#ifdef __GATEKEEPER_TASK_H

#include "FreeRTOS.h"
#include "queue.h"
#include "ui_state.h"

#define gatekeeper_watchdogMIN_TIME 0 // Since gatekeeper is not periodic
#define gatekeeper_watchdogMAX_TIME 700
#define gatekeeperQUEUE_MAX_WAIT_MS 100 // Max blocking time for task (needed for watchdog)
#define gatekeeperQUEUE_LENGHT 10       // Max lenght of the queue
#define gatekeeperMAX_TASK_PER_PAGE 5
#define gatekeeperMAX_TASK (gatekeeperMAX_TASK_PER_PAGE * pageMAX_PAGES) // Max number of tasks that gatekeeper can store
#define gatekeeperTASK_DATA_MAX_LENGHT 32                                // Max lenght of char* that was refered into queue
#define gatekeeperTASK_NAME_MAX_LENGHT 8                                 // Max lenght of the name of the function

extern QueueHandle_t gatekeeper_queueHandle;

typedef struct
{
    int task_id;
    char data[gatekeeperTASK_DATA_MAX_LENGHT];
} GatekeeperQueueData_t;

typedef struct
{
    const char task_names[gatekeeperMAX_TASK][gatekeeperTASK_NAME_MAX_LENGHT];
} GatekeeperTaskConfig_t;

void gatekeeper_init_queue();
void gatekeeper_task(void *pvParameters);

#endif