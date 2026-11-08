#include "fault_hooks.h"

#include "fault_diag.h"

#include "stm32f4xx.h"

#include "FreeRTOS.h"
#include "task.h"

extern char fault_task_name[];
volatile const char *g_assert_file;
volatile int g_assert_line;

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    (void)xTask;
    __disable_irq(); // Disable interrupts
    uint32_t i = 0;

    while (i < (fault_diagFAULT_TASK_MAX_NAME - 1) && pcTaskName[i] != '\0')
    {
        fault_task_name[i] = pcTaskName[i]; // Rewriting into fault_task_name to debug after
        i++;
    }
    fault_task_name[i] = '\0';
    NVIC_SystemReset(); // Reset. Flag will be in RCC->CSR
}
void vApplicationMallocFailedHook(void)
{
    __disable_irq();
    NVIC_SystemReset();
}
void vAssertCalled(const char *file, int line)
{
    __disable_irq();
    g_assert_file = file;
    g_assert_line = line;
#ifdef DEBUG_HALT
    for (;;)
    {
    }
#else
    NVIC_SystemReset();
#endif
}