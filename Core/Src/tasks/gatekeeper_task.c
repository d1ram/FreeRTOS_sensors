#include "gatekeeper_task.h"
#include "watchdog_service.h"

#include "FreeRTOS.h"
#include "ssd1306.h"
#include "fonts.h"
#include "task.h"

void gatekeeperTask(void *pvParameters)
{
    const uint32_t watchdog_task_id = watchdog_add_task(gatekeeper_watchdogMIN_TIME, gatekeeper_watchdogMAX_TIME);
    const gatekeeperTask_Config_t *config = (gatekeeperTask_Config_t *)pvParameters;

    const QueueHandle_t queueHanler = config->queueHandler;
    const char taskNames[gatekeeperMAX_TASK][gatekeeperCHAR_MAX_LENGHT] = config->taskNames;

    const char taskData[gatekeeperMAX_TASK][gatekeeperCHAR_MAX_LENGHT];

    for (;;)
    {
        // Write type: taskNames[taskId] : taskData[taskId]

        gatekeeperQueueData_t data;
        if (xQueueReceive(queueHanler, &data, pdMS_TO_TICKS(gatekeeperQUEUE_MAX_WAIT_MS)) == pdPASS) {
            // function for writing into monitor.
        }
        watchdog_ping(watchdog_task_id, xTaskGetTickCount()); // Ping for watchdog
    }
}