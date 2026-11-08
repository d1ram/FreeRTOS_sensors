#include "gyro_print_task.h"

#include "watchdog_service.h"
#include "gyro_service.h"
#include "ui_state.h" // For queue

#include "FreeRTOS.h"
#include "task.h"

#include "stack_size.h"

#include <stdio.h>

void gyro_print_task(void *pvParameters)
{
    GyroPrintTaskConfig_t *config = (GyroPrintTaskConfig_t *)pvParameters;

    if (config == NULL ||
        gatekeeper_queueHandle == NULL)
    {
        vTaskDelete(NULL);
    }

    int taskId = config->task_id_gatekeeper;
    GyroAxis_t axis = config->data_axis;
    const uint32_t watchdog_task_id = config->watchdog_id;

    for (;;)
    {
        GyroTypeData_t measurement = gyro_get_data(axis);

        GatekeeperQueueData_t data;
        data.task_id = taskId;

        snprintf(data.data, sizeof(data.data), "%d - %d", (int)(measurement.num_1 * 10), (int)(measurement.num_2 * 100));

        xQueueSendToBack(gatekeeper_queueHandle, &data, pdMS_TO_TICKS(gyro_print_taskQUEUE_WAITING_MS));

        watchdog_ping(watchdog_task_id, xTaskGetTickCount(), OK);

        // g_stack_free[taskId] = uxTaskGetStackHighWaterMark(NULL);

        vTaskDelay(pdMS_TO_TICKS(gyro_print_taskREFRESH_MS));
    }
}