#define __PAGE_TASK_H
#ifdef __PAGE_TASK_H

#include "FreeRTOS.h"
#include "task.h"

extern TaskHandle_t page_task_handle;

void page_task(void *pvParameters);
#endif