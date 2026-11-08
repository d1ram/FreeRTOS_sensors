#ifndef __FAULT_HOOKS_H
#define __FAULT_HOOKS_H

#include "stm32f4xx.h"
#include "FreeRTOS.h"
#include "task.h"

extern volatile const char *g_assert_file;
extern volatile int g_assert_line;

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName);
void vApplicationMallocFailedHook(void);
void vAssertCalled(const char *file, int line);
#endif