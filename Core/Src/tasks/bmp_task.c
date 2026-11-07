#include "bmp_task.h"

#include "bmp280_service.h"
#include "watchdog_service.h"

#include "FreeRTOS.h"
#include "task.h"

#include "jitter_test.h"
#include "stack_size.h"

void bmp_task(void *pvParameters)
{
    const uint32_t watchdog_task_id = ((BmpTaskConfig_t *)pvParameters)->watchdog_id;

    TickType_t wake_timeTICKS = xTaskGetTickCount();
    for (;;)
    {
        HAL_StatusTypeDef status = bmp_service_read_data();
        if (status == HAL_OK || status == HAL_BUSY)
        {
            watchdog_ping(watchdog_task_id, xTaskGetTickCount(), OK);
        }
        else if (status == HAL_TIMEOUT)
        {
            watchdog_ping(watchdog_task_id, xTaskGetTickCount(), BUS_ERROR);
        }
        else
        {
            watchdog_ping(watchdog_task_id, xTaskGetTickCount(), SENSOR_ERROR);
        }

        // MARK_BMP_LOW();
        // g_stack_free[3] = uxTaskGetStackHighWaterMark(NULL);
        vTaskDelayUntil(&wake_timeTICKS, pdMS_TO_TICKS(bmp_taskREFRESH_RATE_MS));
        // MARK_BMP_HIGH();
    }
}