#include "watchdog_service.h"

#include "stm32f4xx.h"

#include <stddef.h>
#include <stdio.h>

static WatchdogTask_t g_tasks[watchdogMAX_TASKS];
static uint32_t g_task_counter = 0;

void watchdog_init(void) // Initialization of watchdog registers.
{
    IWDG->KR = watchdogKEY_START;         // 0xCCCC: start LSI
    IWDG->KR = watchdogKEY_PR_RLP_ACCESS; // 0x5555: allow write for PR RLP
    IWDG->PR = IWDG_PR_PR_1;              // divider
    IWDG->RLR = 1249;
    while (IWDG->SR != 0)
    {
    }
}
int32_t watchdog_add_task(uint32_t minTime, uint32_t maxTime, char *task_name) // Returns task_id of watchdog structure of task, otherwise -1.
{
    if (g_task_counter >= watchdogMAX_TASKS)
    {
        return -1;
    }

    uint32_t id = g_task_counter++;
    g_tasks[id].task_id = id;
    g_tasks[id].last_ping = 0;
    g_tasks[id].min_time = minTime;
    g_tasks[id].max_time = maxTime;
    g_tasks[id].has_pinged = false;
    g_tasks[id].is_active = true;
    g_tasks[id].task_name = task_name;

    return id;
}
void watchdog_ping(uint32_t task_id, uint32_t pingTime) // Ping watchdog that task is alive.
{
    if (task_id < watchdogMAX_TASKS && // in bounds of maxTasks and active.
        g_tasks[task_id].is_active)
    {
        if (!g_tasks[task_id].has_pinged || // either first ping or ping > minTime.
            pingTime - g_tasks[task_id].last_ping >= g_tasks[task_id].min_time)
        {
            g_tasks[task_id].last_ping = pingTime; // refresh pingTime.
            g_tasks[task_id].has_pinged = true;
        }
        else
        { // fatal crash (spam)
            g_tasks[task_id].is_active = false;
        }
    }
}
const WatchdogTask_t *watchdog_get_task(uint32_t task_id) // Get task with task_id from g_tasks.
{
    if (task_id >= watchdogMAX_TASKS)
    {
        return NULL;
    }

    return &g_tasks[task_id];
}
uint32_t watchdog_get_counter(void) // Get counter of watchdog g_tasks.
{
    return g_task_counter;
}
void watchdog_refresh_timer(void) // Refresh bytes in IWDG CSMIS.
{
    IWDG->KR = watchdogKEY_REFRESH;
}