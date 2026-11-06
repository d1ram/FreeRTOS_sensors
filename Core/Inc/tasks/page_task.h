#ifndef __PAGE_TASK_H
#define __PAGE_TASK_H

#include "FreeRTOS.h" // For task.h
#include "task.h"

extern TaskHandle_t page_task_handle;

void page_task(void *pvParameters);
#endif