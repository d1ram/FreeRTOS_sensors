#include "watchdog_task.h"
#include "watchdog_service.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdbool.h>

void task_watchdog_check(void *pvParameters)
{
    const uint32_t timer = pdMS_TO_TICKS(watchdogTIMER_MS);

    uint32_t task_count;
    for (;;)
    {
        task_count = watchdog_get_counter();
        bool all_tasks_alive = true;
        uint32_t currentTime = xTaskGetTickCount();

        for (uint32_t task_id = 0; task_id < task_count; task_id++)
        {
            const WatchdogTaskType *task = watchdog_get_task(task_id);

            if (task != NULL)
            {
                uint32_t elapsed = currentTime - task->lastPing;
                if (elapsed > task->maxTime || !task->is_active)
                {
                    all_tasks_alive = false;
                    break;
                }
            }
        }

        if (all_tasks_alive)
        {
            watchdog_refresh_timer();
        }
        else
        {
        }

        vTaskDelay(timer);
    }
}