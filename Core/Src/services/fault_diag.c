#include "fault_diag.h"

#include "stm32f4xx_hal.h"

#include <stdio.h>

__attribute__((section(".noinit"))) char fault_task_name[fault_diagFAULT_TASK_MAX_NAME];
static SystemResetInfo_t info_g;

void fault_diag_init(void)
/*
1. Get RCC Flag.
2. Store it in info.
3. If RCC flag == IWDG (watchdog timer) -> fault_task_name into info (fault_task_name written in watchdog_task).
*/
{
    info_g.faulty_task[0] = '\0';
    if (__HAL_RCC_GET_FLAG(RCC_FLAG_SFTRST))
    {
        info_g.cause = RESET_CAUSE_SOFTWARE;

        if (fault_task_name[0] != '\0') // Might be StackOverflow.
        {
            snprintf(info_g.faulty_task, sizeof(info_g.faulty_task), "%s", fault_task_name);
        }
    }
    else if (__HAL_RCC_GET_FLAG(RCC_FLAG_IWDGRST))
    {
        info_g.cause = RESET_CAUSE_IWDG;

        if (fault_task_name[0] != '\0')
        {
            snprintf(info_g.faulty_task, sizeof(info_g.faulty_task), "%s", fault_task_name);
        }
    }
    else if (__HAL_RCC_GET_FLAG(RCC_FLAG_PORRST))
    {
        info_g.cause = RESET_CAUSE_POWER_ON;
    }
    else if (__HAL_RCC_GET_FLAG(RCC_FLAG_PINRST))
    {
        info_g.cause = RESET_CAUSE_PIN;
    }
    else
    {
        info_g.cause = RESET_CAUSE_UNKNOWN;
    }

    fault_task_name[0] = '\0';

    __HAL_RCC_CLEAR_RESET_FLAGS();
}

const SystemResetInfo_t *fault_get_info(void)
{
    return &info_g;
}
