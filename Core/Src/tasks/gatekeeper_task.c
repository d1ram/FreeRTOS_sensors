#include "gatekeeper_task.h"

#include "watchdog_service.h"
#include "monitor_service.h"

#include "FreeRTOS.h"
#include "ssd1306.h"
#include "fonts.h"
#include "task.h"

#include <stdio.h>

QueueHandle_t gatekeeper_queueHandle = NULL;
void gatekeeper_init_queue()
{
    gatekeeper_queueHandle = xQueueCreate(gatekeeperQUEUE_LENGHT, sizeof(gatekeeperQueueData_t));
}

void gatekeeper_task(void *pvParameters)
{
    const uint32_t watchdog_task_id = watchdog_add_task(gatekeeper_watchdogMIN_TIME, gatekeeper_watchdogMAX_TIME);
    if (watchdog_task_id == -1)
    {
        vTaskDelete(NULL);
    }

    const gatekeeperTaskConfig_t *config = (gatekeeperTaskConfig_t *)pvParameters;

    if (config == NULL || gatekeeper_queueHandle == NULL)
    {
        monitor_write_line(2, "DELETED");
        vTaskDelete(NULL);
    }

    const char (*taskNames)[gatekeeperTASK_NAME_MAX_LENGHT] = config->taskNames;

    for (;;)
    {
        // Write type: "<taskName>: <taskData>"
        gatekeeperQueueData_t data;
        if (xQueueReceive(gatekeeper_queueHandle, &data, pdMS_TO_TICKS(gatekeeperQUEUE_MAX_WAIT_MS)) == pdPASS)
        {
            // Function for writing into monitor.
            char buf[gatekeeperTASK_NAME_MAX_LENGHT + gatekeeperTASK_DATA_MAX_LENGHT + 2]; // buf for "<taskName>: <taskData>"
            if (data.taskId >= 0 && data.taskId < gatekeeperMAX_TASK)
            {
                snprintf(buf, sizeof(buf), "%s: %s", taskNames[data.taskId], data.data);
                monitor_write_line(data.taskId, buf);
            }
        }

        watchdog_ping(watchdog_task_id, xTaskGetTickCount()); // Ping for watchdog
    }
}