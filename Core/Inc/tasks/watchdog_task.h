#include <stdint.h>
#include "watchdog_service.h"

#ifndef __WATCHDOG_TASK_H
#define __WATCHDOG_TASK_H

#define watchdogTIMER_MS 100

void task_watchdog_check(void *pvParameters);
#endif