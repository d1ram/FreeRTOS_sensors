#include "page_task.h"

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
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY); // Always in blocked state until notified from ISR.
        current_page_g = (current_page_g + 1) % pageMAX_PAGES;
        // g_stack_free[5] = uxTaskGetStackHighWaterMark(NULL);
    }
}