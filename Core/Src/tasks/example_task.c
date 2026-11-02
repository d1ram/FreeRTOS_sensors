#include "example_task.h"

#include "watchdog_service.h"
#include "FreeRTOS.h"
#include "task.h"
#include "stm32f4xx_hal.h"
#include "monitor_service.h"
#include <stdio.h>
#include "queue.h"
#include "gatekeeper_task.h"

#include <stdbool.h>

void example_task(void *pvParameters)
{
    uint32_t minTime = pdMS_TO_TICKS(900);
    uint32_t maxTime = pdMS_TO_TICKS(1150);

    uint32_t watchdog_task_id = watchdog_add_task(minTime, maxTime);

    if (watchdog_task_id == -1)
    {
        vTaskDelete(NULL);
    }
    // for (;;)
    // {
    //     HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);

    //     watchdog_ping(watchdog_task_id, xTaskGetTickCount());
    //     vTaskDelay(pdMS_TO_TICKS(500));
    // }

    // int j = 0;
    // for (;;)
    // {
    //     for (int i = 0; i < 6; i++)
    //     {
    //         const char *str = "popa: ";
    //         char buf[12];
    //         snprintf(buf, sizeof(buf), "%s%d", str, j++);
    //         monitor_write_line(i, buf);
    //         vTaskDelay(pdMS_TO_TICKS(100));
    //     }
    // }

    const example_t *example = pvParameters;

    if (example == NULL ||
        gatekeeper_queueHandle == NULL)
    {
        monitor_write_line(4, "NULL");
        vTaskDelete(NULL);
    }

    const int testId = example->taskId;
    int testNum = 1 - testId;

    char *testData;
    if (testId == 0)
    {
        testData = "2";
    }
    else
    {
        testData = "7";
    }
    for (;;)
    {
        gatekeeperQueueData_t data;
        data.taskId = testId;
        snprintf(data.data, sizeof(data.data), "%s%d C", testData, testNum++);
        xQueueSendToBack(gatekeeper_queueHandle, &data, 100);
        testNum = testNum % 8;
        watchdog_ping(watchdog_task_id, xTaskGetTickCount());
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}