#include "watchdog_task.h"

#include "watchdog_service.h"
#include "fault_diag.h"

#include "FreeRTOS.h"
#include "task.h"

#include "ui_state.h"

#include "stack_size.h"

#include <stdbool.h>
#include <stdio.h>

void watchdog_task(void *pvParameters)
{
    const uint32_t timer = pdMS_TO_TICKS(watchdogTIMER_MS);

    WatchdogConfig_t *config = (WatchdogConfig_t *)pvParameters;

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

                if (config != NULL)
                {
                    for (int32_t i = 0; i < config->task_count; i++)
                    {
                        if (task_id == config->mon_tasks[i].task_watchdog_id)
                        {
                            GatekeeperQueueData_t data = {
                                .task_id = config->mon_tasks[i].task_gatekeeper_id};

                            if (task->status == OK)
                            {
                                snprintf(data.data, sizeof(data.data), "OK");
                            }
                            else if (task->status == SENSOR_ERROR)
                            {
                                snprintf(data.data, sizeof(data.data), "S_Err");
                            }
                            else
                            {
                                snprintf(data.data, sizeof(data.data), "B_Err");
                            }

                            xQueueSendToBack(gatekeeper_queueHandle, &data, pdMS_TO_TICKS(10)); // 3rd page status show.
                            break;
                        }
                    }
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

#ifdef DIAG
        g_stack_free[4] = uxTaskGetStackHighWaterMark(NULL);
#endif
    }
}