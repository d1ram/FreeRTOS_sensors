#include <stdint.h>
#include <stdbool.h>

#ifndef __WATCHDOG_H
#define __WATCHDOG_H

#define watchdogKEY_START 0xCCCC
#define watchdogKEY_REFRESH 0xAAAA
#define watchdogKEY_PR_RLP_ACCESS 0x5555

#define watchdogMAX_TASKS 16

struct
{
    uint32_t task_id;
    uint32_t minTime;
    uint32_t maxTime;
    uint32_t lastPing;
    bool is_active;
} typedef WatchdogTaskType;

void watchdog_init(void);
uint32_t watchdog_add(uint32_t minTime, uint32_t maxTime);
void watchdog_ping(uint32_t task_id, uint32_t pingTime);

#endif