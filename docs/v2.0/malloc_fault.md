## configUSE_MALLOC_FAILED_HOOK 1

### Overview

`fault_hooks.c` now contains `void vApplicationMallocFailedHook(void)`, which is a hook for malloc. Main idea: if malloc fail happens in task, this hook will call `NVIC_SystemReset()`.

### Why?

1. Sometimes when we try to call `malloc()`, we get `NULL` (for example, if not enough heap exists). In that case, if memory is not checked task can go onto the `NULL` address and immediately go into `Hard_Reset`. For that case we have this hook, that will allow us to control and not let that situation go.
2. Sometimes when we call `malloc()`, we get our space from heap. Heap has limited space, so if we have no function `free_malloc()`, heap will be fully taken. In that case, when `malloc()` will be called at the last time and will not be able to return new free space, it will call our hook `vApplicationMallocFailedHook()`.

### Testing

```
for (;;)
{
    volatile uint8_t *p = pvPortMalloc(256);
    if (p == NULL)
    {
        vTaskSuspend(NULL);
    }
    p[0] = 0xAA;
    vTaskDelay(pdMS_TO_TICKS(10));
}
```

After approximately 2 seconds plate will restart.
