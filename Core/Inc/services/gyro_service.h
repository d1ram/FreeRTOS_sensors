#ifndef __GYRO_SERVICE_H
#define __GYRO_SERVICE_H

#include "stm32f4xx_hal.h"

#define gyroMAX_MUTEX_TIMEOUT_MS 100

typedef enum
{
    X = 0,
    Y,
    Z
} GyroAxis_t; // Data for gyro_get_data() func
typedef struct
{
    float num_1;  // GYRO_NUM
    float num_2;  // ACCEL_NUM
} GyroTypeData_t; // For one of the axis.

HAL_StatusTypeDef gyro_service_init(I2C_HandleTypeDef *bus);
HAL_StatusTypeDef gyro_read_data(void);
GyroTypeData_t gyro_get_data(GyroAxis_t type);
#endif