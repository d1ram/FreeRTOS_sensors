#include <stdint.h>
#include <stdbool.h>

#ifndef __WATCHDOG_SERVICE_H
#define __WATCHDOG_SERVICE_H

#define watchdogKEY_START 0xCCCC
#define watchdogKEY_REFRESH 0xAAAA
#define watchdogKEY_PR_RLP_ACCESS 0x5555

#define watchdogMAX_TASKS 16

struct
{
    uint32_t task_id;
    uint32_t minTime;
    uint32_t maxTime;
    volatile uint32_t lastPing;
    volatile bool hasPinged;
    volatile bool is_active;
} typedef WatchdogTask_t;

void watchdog_init(void);
uint32_t watchdog_add_task(uint32_t minTime, uint32_t maxTime);
void watchdog_ping(uint32_t task_id, uint32_t pingTime);

const WatchdogTask_t *watchdog_get_task(uint32_t task_id);
uint32_t watchdog_get_counter(void);

void watchdog_refresh_timer(void);

#endif