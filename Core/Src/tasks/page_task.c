#include "page_task.h"

#include "stm32f4xx_hal.h"

#include "ui_state.h"

#include "FreeRTOS.h"
#include "task.h"

#include "stack_size.h"

volatile int current_page_g = 0;      // From ui_state.h
TaskHandle_t page_task_handle = NULL; // For handler

void page_task(void *pvParameters)
{
    page_task_handle = xTaskGetCurrentTaskHandle();
    for (;;)
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);              // Always in blocked state until notified from ISR.
        vTaskDelay(pdMS_TO_TICKS(page_taskDEBOUNCE_TIME_MS)); // Wait in case of jitter.
        if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET)
        {
            current_page_g = (current_page_g + 1) % pageMAX_PAGES;

            while (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET) // Wait for letting button go
            {
                vTaskDelay(pdMS_TO_TICKS(10));
            }
            vTaskDelay(pdMS_TO_TICKS(page_taskDEBOUNCE_TIME_MS)); // Jitter on getting off the btn
        }
        ulTaskNotifyTake(pdTRUE, 0);
        // g_stack_free[5] = uxTaskGetStackHighWaterMark(NULL);
    }
}