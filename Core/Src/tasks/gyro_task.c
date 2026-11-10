#include "gyro_task.h"

#include "gyro_service.h"
#include "watchdog_service.h"

#include "FreeRTOS.h"
#include "task.h"

#include "jitter_test.h"
#include "stack_size.h"

void gyro_task(void *pvParameters)
{
    const uint32_t watchdog_task_id = ((GyroTaskConfig_t *)pvParameters)->watchdog_id;

    TickType_t wake_timeTICKS = xTaskGetTickCount();
    for (;;)
    {
        HAL_StatusTypeDef status = gyro_read_data();
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

#ifdef DIAG
        g_stack_free[9] = uxTaskGetStackHighWaterMark(NULL);
#endif

        vTaskDelayUntil(&wake_timeTICKS, pdMS_TO_TICKS(gyro_taskREFRESH_RATE_MS));
    }
}