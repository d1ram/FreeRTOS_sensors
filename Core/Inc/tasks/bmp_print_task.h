#ifndef __BMP_PRINT_TASK_H
#define __BMP_PRINT_TASK_H

#define bmp_print_task_watchdogMIN_TIME_MS 750
#define bmp_print_task_watchdogMAX_TIME_MS 2000 // Since can be more than 1, better to make it big.
#define bmp_print_taskREFRESH_MS 800
#define bmp_print_taskQUEUE_WAITING_MS 100

typedef float (*BMP_Task_Function)(void);

typedef struct
{
    int task_id_gatekeeper;
    BMP_Task_Function function;
    char *d_unit;
    int watchdog_id;
} BmpPrintTaskConfig_t;

void bmp_print_task(void *pvParameters);
#endif