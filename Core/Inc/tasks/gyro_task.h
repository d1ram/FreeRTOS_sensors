// Refresh data on gyro
#ifndef __GYRO_TASK_H
#define __GYRO_TASK_H

#define gyro_task_watchdogMIN_TIME_MS 200
#define gyro_task_watchdogMAX_TIME_MS 800

#define gyro_taskREFRESH_RATE_MS 300

typedef struct
{
    int watchdog_id;
} GyroTaskConfig_t;

void gyro_task(void *pvParameters);

#endif