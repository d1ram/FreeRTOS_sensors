#ifndef __PAGE_TASK_H
#define __PAGE_TASK_H

#include "FreeRTOS.h" // For task.h
#include "task.h"

#define page_taskDEBOUNCE_TIME_MS 30 // For how long button should be pressed for press-registration
extern TaskHandle_t page_task_handle;

void page_task(void *pvParameters);
#endif