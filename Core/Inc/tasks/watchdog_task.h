// Watchdog task that controls the work of registered tasks.
#include <stdint.h>
#include "watchdog_service.h"

#ifndef __WATCHDOG_TASK_H
#define __WATCHDOG_TASK_H

#define watchdogTIMER_MS 100

void watchdog_task(void *pvParameters);
#endif