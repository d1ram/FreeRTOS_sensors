#include "FreeRTOS.h"
#include "queue.h"
#include <stdio.h>
#ifndef __EXAMPLE_TASK_H
#define __EXAMPLE_TASK_H

typedef struct
{
    int taskId;
} example_t;

void example_task(void *pvParameters);
#endif