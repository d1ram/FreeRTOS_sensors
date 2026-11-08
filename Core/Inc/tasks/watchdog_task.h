// Watchdog task that controls the work of registered tasks.
#ifndef __WATCHDOG_TASK_H
#define __WATCHDOG_TASK_H

#include <stdint.h>

#define watchdogTIMER_MS 100
#define watchdogMAX_MONITORED_TASKS 2

typedef struct
{
    int32_t task_watchdog_id;
    int32_t task_gatekeeper_id;
} WatchdogTaskMapping_t;

typedef struct // Struct for tasks that will be sent to the gatekeeper queue and be shown on the screen (page 3).
{
    WatchdogTaskMapping_t mon_tasks[watchdogMAX_MONITORED_TASKS];
    int32_t task_count;
} WatchdogConfig_t;

void watchdog_task(void *pvParameters);
#endif