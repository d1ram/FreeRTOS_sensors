// Watchdog service that provides task with every needed.
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
    uint32_t min_time;
    uint32_t max_time;
    volatile uint32_t last_ping;
    volatile bool has_pinged;
    volatile bool is_active;
} typedef WatchdogTask_t;

void watchdog_init(void);
uint32_t watchdog_add_task(uint32_t min_time, uint32_t max_time);
void watchdog_ping(uint32_t task_id, uint32_t ping_time);

const WatchdogTask_t *watchdog_get_task(uint32_t task_id);
uint32_t watchdog_get_counter(void);

void watchdog_refresh_timer(void);

#endif