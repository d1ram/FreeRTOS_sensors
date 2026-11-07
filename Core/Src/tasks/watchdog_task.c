#include "watchdog_task.h"

#include "watchdog_service.h"
#include "fault_diag.h"

#include "FreeRTOS.h"
#include "task.h"

#include "stack_size.h"

#include <stdbool.h>
#include <stdio.h>

void watchdog_task(void *pvParameters)
{
    const uint32_t timer = pdMS_TO_TICKS(watchdogTIMER_MS);

    static bool fault_occured = false; // If even once fault occured - never refreshing timer again.
    uint32_t task_count;
    for (;;)
    {
        vTaskDelay(timer);

        task_count = watchdog_get_counter();
        bool all_tasks_alive = true;
        uint32_t current_time = xTaskGetTickCount();

        for (uint32_t task_id = 0; task_id < task_count; task_id++)
        {
            const WatchdogTask_t *task = watchdog_get_task(task_id);

            if (task != NULL)
            {
                uint32_t elapsed = current_time - task->last_ping;
                if (elapsed > task->max_time || !task->is_active)
                {
                    all_tasks_alive = false;
                    snprintf(fault_task_name, sizeof(fault_task_name), "%s", task->task_name); // Write into fault_task_name (fault_diag.c) task that caused reset.
                    break;
                }
            }
        }

        if (!all_tasks_alive)
        {
            fault_occured = true;
        }

        if (!fault_occured)
        {
            watchdog_refresh_timer();
        }

        // g_stack_free[6] = uxTaskGetStackHighWaterMark(NULL);
    }
}