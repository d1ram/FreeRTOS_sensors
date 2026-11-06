#include "btn_handler.h"

#include "stm32f4xx_hal.h"

#include "ui_state.h" // For page_task_handle

#include "FreeRTOS.h"
#include "task.h"

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == GPIO_PIN_13)
    {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;

        if (page_task_handle != NULL)
        {
            vTaskNotifyGiveFromISR(page_task_handle, &xHigherPriorityTaskWoken);
        }

        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}