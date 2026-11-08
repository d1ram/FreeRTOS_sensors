## configCHECK_FOR_STACK_OVERFLOW 1

### Overview

`fault_hooks.c` now contains `void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)`, which is a hook for stack overflow. Main idea: if stack is too low for a task, this hook will call `NVIC_SystemReset()` and save `fault_task_name`.

### Why?

Sometimes when allocated stack for a task is too small, task will go into others fields of memory that is not connected to this task. It can be junk data, data of another tasks, services and exc. And sometimes it can lead to really hard debugging process, because it can occur after a long time (or never occur at all, but work incorrectly). `configCHECK_FOR_STACK_OVERFLOW` ensures us, that if task goes outside of it's allocated stack, `vApplicationStackOverflowHook` will be called and system will be reset with the `fault_task_name` saved for debugging.

### How tested

Wrote simple task:

```
#include "example_task.h"

#include "FreeRTOS.h"
#include "task.h"
#include "gatekeeper_task.h"

void example_task(void *pvParameters)
{
    (void)pvParameters;

    for (;;)
    {
        vTaskDelay(pdMS_TO_TICKS(5000));

        volatile char heavy_stack_consumer[500];

        for (int i = 0; i < 500; i++)
        {
            heavy_stack_consumer[i] = 0xAA;
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
```

That has only `128 words` allocated in `main.c`:

```
xTaskCreate(example_task, "exampleTASK", 128, NULL, 1, NULL);
```

Now everytime i try to run my STM32 it goes into the HardReset immediatly (because allocation goes on compilation stage). Reason in this situation is `SO`, or `RESET_CAUSE_SOFTWARE`, which is a state of `RCC` -> `RCC_FLAG_SFTRST`.
