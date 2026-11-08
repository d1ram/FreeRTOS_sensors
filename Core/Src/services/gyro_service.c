#include "gyro_service.h"

#include "MPU6050.h"

#include "i2c_bus.h"

static float ax, ay, az;
static float gx, gy, gz;

static I2C_HandleTypeDef *s_hi2c = NULL;

HAL_StatusTypeDef gyro_service_init(I2C_HandleTypeDef *bus)
{
    HAL_StatusTypeDef status;

    s_hi2c = bus;
    status = MPU6050_init(bus);
    return status;
}
HAL_StatusTypeDef gyro_read_data(void)
{
    HAL_StatusTypeDef status = HAL_TIMEOUT;

    if (i2c_mutex_take(pdMS_TO_TICKS(gyroMAX_MUTEX_TIMEOUT_MS)) == pdTRUE)
    {
        status = MPU6050_Read_Accel(&ax, &ay, &az);
        if (status == HAL_OK)
        {
            status = MPU6050_Read_Gyro(&gx, &gy, &gz);
        }

        if (status == HAL_TIMEOUT)
        {
            i2c_bus_recover(s_hi2c);
        }

        i2c_mutex_give();
    }

    return status;
}
GyroTypeData_t gyro_get_data(GyroAxis_t type)
{
    if (type == X)
    {
        return (GyroTypeData_t){gx, ax};
    }
    else if (type == Y)
    {
        return (GyroTypeData_t){gy, ay};
    }
    return (GyroTypeData_t){gz, az};
}