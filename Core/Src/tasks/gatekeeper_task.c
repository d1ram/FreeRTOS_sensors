#include "gatekeeper_task.h"

#include "ui_state.h"
#include "watchdog_service.h"
#include "monitor_service.h"
#include "fault_diag.h"

#include "FreeRTOS.h"
#include "ssd1306.h"
#include "fonts.h"
#include "task.h"

#include "stack_size.h"

#include <stdio.h>
#include <string.h>

QueueHandle_t gatekeeper_queueHandle = NULL; // From ui_state.h
void gatekeeper_init_queue()
{
    gatekeeper_queueHandle = xQueueCreate(gatekeeperQUEUE_LENGHT, sizeof(GatekeeperQueueData_t));
    configASSERT(gatekeeper_queueHandle != NULL);
}

void gatekeeper_task(void *pvParameters)
{
    const GatekeeperTaskConfig_t *config = (GatekeeperTaskConfig_t *)pvParameters;

    if (config == NULL || gatekeeper_queueHandle == NULL)
    {
        monitor_set_line(2, "DELETED");
        monitor_flush();
        vTaskDelete(NULL);
    }

    char (*task_names)[gatekeeperTASK_NAME_MAX_LENGHT] = config->task_names;
    const uint32_t watchdog_task_id = config->watchdog_id;

    char task_data[gatekeeperMAX_TASK][gatekeeperTASK_DATA_MAX_LENGHT] = {0}; // '\0'
    int last_page = -1;                                                       // So at the first iteration fullscreen will be drawn.

    // STATUS_NAMES
    const int bmp_status_id = gatekeeperMAX_TASK_PER_PAGE * 2;
    const int gyro_status_id = (gatekeeperMAX_TASK_PER_PAGE * 2) + 1;
    snprintf(task_names[bmp_status_id], gatekeeperTASK_NAME_MAX_LENGHT, "BMP_S");
    snprintf(task_names[gyro_status_id], gatekeeperTASK_NAME_MAX_LENGHT, "GYRO_S");
    // FAULT_DIAG
    const SystemResetInfo_t *reset_info = fault_get_info();
    const int cause_id = (gatekeeperMAX_TASK_PER_PAGE * 2) + 2; // 3 page, line 2;

    snprintf(task_names[cause_id], gatekeeperTASK_NAME_MAX_LENGHT, "RC");
    if (reset_info->cause == RESET_CAUSE_IWDG)
    {
        if (reset_info->faulty_task[0] != '\0')
        {
            const int cause_task_id = (gatekeeperMAX_TASK_PER_PAGE * 2) + 3; // 3 page, line 3;
            snprintf(task_names[cause_task_id], gatekeeperTASK_NAME_MAX_LENGHT, "TASK");
            snprintf(task_data[cause_task_id], gatekeeperTASK_DATA_MAX_LENGHT, "%s", reset_info->faulty_task);
        }

        snprintf(task_data[cause_id], gatekeeperTASK_DATA_MAX_LENGHT, "IWDG");
    }
    else if (reset_info->cause == RESET_CAUSE_PIN)
    {
        snprintf(task_data[cause_id], gatekeeperTASK_DATA_MAX_LENGHT, "PIN");
    }
    else if (reset_info->cause == RESET_CAUSE_POWER_ON)
    {
        snprintf(task_data[cause_id], gatekeeperTASK_DATA_MAX_LENGHT, "PO");
    }
    else if (reset_info->cause == RESET_CAUSE_SOFTWARE)
    {
        if (reset_info->faulty_task[0] != '\0') // If StackOverflow
        {
            const int cause_task_id = (gatekeeperMAX_TASK_PER_PAGE * 2) + 3;
            snprintf(task_names[cause_task_id], gatekeeperTASK_NAME_MAX_LENGHT, "TASK");
            snprintf(task_data[cause_task_id], gatekeeperTASK_DATA_MAX_LENGHT, "%s", reset_info->faulty_task);
        }

        snprintf(task_data[cause_id], gatekeeperTASK_DATA_MAX_LENGHT, "SO");
    }
    else
    {
        snprintf(task_data[cause_id], gatekeeperTASK_DATA_MAX_LENGHT, "UN");
    }

    for (;;)
    {
        HAL_StatusTypeDef status = HAL_OK;
        if (current_page_g != last_page)
        {
            last_page = current_page_g;

            int first_element = current_page_g * gatekeeperMAX_TASK_PER_PAGE;
            int current_line = 0;
            char buf[gatekeeperTASK_NAME_MAX_LENGHT + gatekeeperTASK_DATA_MAX_LENGHT + 2];
            for (int i = first_element; i < first_element + gatekeeperMAX_TASK_PER_PAGE; i++)
            {
                if (task_data[i][0] != '\0')
                {
                    snprintf(buf, sizeof(buf), "%s: %s", task_names[i], task_data[i]);
                }
                else
                {
                    snprintf(buf, sizeof(buf), "%s: ---", task_names[i]);
                }
                monitor_set_line(current_line, buf);
                current_line++;
            }
            status = monitor_flush();
        }
        /* Why else? Because if in the same iteration we will update whole monitor and also read from queue, watchdog
        might think that task died. Otherwise we could make max_time for watchdog bigger.  */
        else
        {
            // Write type: "<taskName>: <taskData>"
            GatekeeperQueueData_t data;
            if (xQueueReceive(gatekeeper_queueHandle, &data, pdMS_TO_TICKS(gatekeeperQUEUE_MAX_WAIT_MS)) == pdPASS)
            {
                // Function for writing into monitor.
                char buf[gatekeeperTASK_NAME_MAX_LENGHT + gatekeeperTASK_DATA_MAX_LENGHT + 2]; // buf for "<taskName>: <taskData>"
                if (data.task_id >= 0 && data.task_id < gatekeeperMAX_TASK)
                {
                    strncpy(task_data[data.task_id], data.data, gatekeeperTASK_DATA_MAX_LENGHT);
                    task_data[data.task_id][gatekeeperTASK_DATA_MAX_LENGHT - 1] = '\0';

                    int task_page = data.task_id / gatekeeperMAX_TASK_PER_PAGE;
                    if ((data.task_id / gatekeeperMAX_TASK_PER_PAGE) == current_page_g) // If on exact page of it
                    {
                        int task_page_id = data.task_id - (task_page * gatekeeperMAX_TASK_PER_PAGE);
                        snprintf(buf, sizeof(buf), "%s: %s", task_names[data.task_id], data.data);
                        monitor_set_line(task_page_id, buf);
                        status = monitor_flush();
                    }
                }
            }
        }

        // g_stack_free[4] = uxTaskGetStackHighWaterMark(NULL);

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
    }
}