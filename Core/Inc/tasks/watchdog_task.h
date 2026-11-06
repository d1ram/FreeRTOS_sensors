// Watchdog task that controls the work of registered tasks.
#ifndef __WATCHDOG_TASK_H
#define __WATCHDOG_TASK_H

#define watchdogTIMER_MS 100

void watchdog_task(void *pvParameters);
#endif