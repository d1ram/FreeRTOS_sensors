#include "bmp_print_task.h"

#include "watchdog_service.h"
#include "ui_state.h" // For queue

#include "FreeRTOS.h"
#include "task.h"

#include "stack_size.h"

#include <stdio.h>

void bmp_print_task(void *pvParameters)
{
    BmpPrintTaskConfig_t *config = (BmpPrintTaskConfig_t *)pvParameters;

    if (config == NULL ||
        gatekeeper_queueHandle == NULL)
    {
        vTaskDelete(NULL);
    }

    int taskId = config->task_id_gatekeeper;
    BMP_Task_Function bmp_get = config->function;
    char *d_unit = config->d_unit;
    const uint32_t watchdog_task_id = config->watchdog_id;

    for (;;)
    {
        float measurement = bmp_get();

        GatekeeperQueueData_t data;
        data.task_id = taskId;

        snprintf(data.data, sizeof(data.data), "%.1d %s", (int)measurement, d_unit);

        xQueueSendToBack(gatekeeper_queueHandle, &data, pdMS_TO_TICKS(bmp_print_taskQUEUE_WAITING_MS));

        watchdog_ping(watchdog_task_id, xTaskGetTickCount(), OK);

        // g_stack_free[taskId] = uxTaskGetStackHighWaterMark(NULL);

        vTaskDelay(pdMS_TO_TICKS(bmp_print_taskREFRESH_MS));
    }
}