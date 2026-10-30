#include "watchdog_service.h"
#include "stm32f410tx.h"

static WatchdogTaskType g_tasks[watchdogMAX_TASKS];
static uint32_t g_task_counter = 0;

void watchdog_init(void)
{
    IWDG->KR = watchdogKEY_PR_RLP_ACCESS; // access to PR and RLP
    IWDG->PR = IWDG_PR_PR_1;              // divider for LSI
    IWDG->RLR = 1249;                     // timeout
    while (IWDG->SR != 0)
        ;
    IWDG->KR = watchdogKEY_START;
}
uint32_t watchdog_add(uint32_t minTime, uint32_t maxTime)
{
    if (g_task_counter >= watchdogMAX_TASKS)
    {
        return -1;
    }

    uint32_t id = g_task_counter++;
    g_tasks[id].task_id = id;
    g_tasks[id].minTime = minTime;
    g_tasks[id].maxTime = maxTime;
    g_tasks[id].is_active = true;

    return id;
}
void watchdog_ping(uint32_t task_id, uint32_t pingTime)
{
    if (task_id >= 0 && g_tasks[task_id].is_active)
    {
        g_tasks[task_id].lastPing = pingTime;
    }
}

// TickType_t current_time = xTaskGetTickCount();
