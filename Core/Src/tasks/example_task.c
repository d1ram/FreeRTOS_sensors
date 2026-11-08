// #include "example_task.h"

// #include "FreeRTOS.h"
// #include "task.h"
// #include "gatekeeper_task.h"

// void example_task(void *pvParameters)
// {
//     (void)pvParameters;

//     for (;;)
//     {
//         volatile uint8_t *p = pvPortMalloc(256);
//         if (p == NULL)
//         {
//             vTaskSuspend(NULL);
//         }
//         p[0] = 0xAA;
//         vTaskDelay(pdMS_TO_TICKS(10));
//     }
// }