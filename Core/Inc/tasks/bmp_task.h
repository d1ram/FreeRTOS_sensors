// Refresh data on bmp
#ifndef __BMP_TASK_H
#define __BMP_TASK_H

#define bmp_task_watchdogMIN_TIME_MS 200
#define bmp_task_watchdogMAX_TIME_MS 800

#define bmp_taskREFRESH_RATE_MS 300 // Once in bmp_taskREFRESH_RATE_MS bmp_task will update data

void bmp_task(void *pvParameters);

#endif