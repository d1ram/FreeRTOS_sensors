#include "bmp_task.h"
#include "FreeRTOS.h"
#include "task.h"
#include "bmp280_service.h"
#include "watchdog_service.h"

void bmp_task(void *pvParameters)
{
    uint32_t minTime = pdMS_TO_TICKS(bmp_task_watchdogMIN_TIME_MS);
    uint32_t maxTime = pdMS_TO_TICKS(bmp_task_watchdogMAX_TIME_MS);

    uint32_t watchdog_task_id = watchdog_add_task(minTime, maxTime);

    if (watchdog_task_id == -1)
    {
        vTaskDelete(NULL);
    }

    TickType_t wake_timeTICKS = xTaskGetTickCount();
    for (;;)
    {
        bmp_service_read_data();
        watchdog_ping(watchdog_task_id, xTaskGetTickCount());
        vTaskDelayUntil(&wake_timeTICKS, pdMS_TO_TICKS(bmp_taskREFRESH_RATE_MS));
    }
}