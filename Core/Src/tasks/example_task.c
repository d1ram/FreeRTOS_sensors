#include "example_task.h"

#include "watchdog_service.h"
#include "FreeRTOS.h"
#include "task.h"
#include "stm32f4xx_hal.h"

#include <stdbool.h>

void example_task(void *pvParameters)
{
    uint32_t minTime = pdMS_TO_TICKS(450);
    uint32_t maxTime = pdMS_TO_TICKS(550);

    uint32_t watchdog_task_id = watchdog_add_task(minTime, maxTime);

    if (watchdog_task_id == -1)
    {
        vTaskDelete(NULL);
    }
    for (;;)
    {
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);

        watchdog_ping(watchdog_task_id, xTaskGetTickCount());
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}