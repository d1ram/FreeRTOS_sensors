#ifndef __GYRO_PRINT_TASK_H
#define __GYRO_PRINT_TASK_H

#include "gyro_service.h"

#define gyro_print_task_watchdogMIN_TIME_MS 750
#define gyro_print_task_watchdogMAX_TIME_MS 2000
#define gyro_print_taskREFRESH_MS 800
#define gyro_print_taskQUEUE_WAITING_MS 100

typedef struct
{
    int task_id_gatekeeper;
    GyroAxis_t data_axis;
    int watchdog_id;
} GyroPrintTaskConfig_t;

void gyro_print_task(void *pvParameters);
#endif