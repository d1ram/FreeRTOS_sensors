// Watchdog task that controls the work of registered tasks.
#ifndef __WATCHDOG_TASK_H
#define __WATCHDOG_TASK_H

#include <stdint.h>

#define watchdogTIMER_MS 100

typedef struct // Struct for tasks that will be sent to the gatekeeper queue and be shown on the screen (page 3).
{
    int32_t bmp_task_watchdog_id;   // Watchdog id
    int32_t bmp_task_gatekeeper_id; // Id of line in gatekeeper
} WatchdogConfig_t;

void watchdog_task(void *pvParameters);
#endif