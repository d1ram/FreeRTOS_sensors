#include "bmp280_service.h"

#include "i2c_bus.h"
#include "jitter_test.h"

#include "BMP280.h"

#include "FreeRTOS.h"

static float temperature = 0.0f;
static float pressure = 0.0f;
static float altitude = 0.0f;

bmp280_t bmp;
static I2C_HandleTypeDef *s_hi2c = NULL;

void bmp_service_init(I2C_HandleTypeDef *bus)
{
    bmp.hi2c = bus;
    s_hi2c = bus;

    bmp.comm_mode = BMP280_MODE_I2C;
    bmp.address = bmpADDRESS;

    bmp280_init(&bmp);
}

HAL_StatusTypeDef bmp_service_read_data(void)
{
    int32_t temp_raw, press_raw;
    HAL_StatusTypeDef status = HAL_TIMEOUT;

    if (i2c_mutex_take(bmpMAX_MUTEX_TIMEOUT_MS) == pdTRUE)
    {
        MARK_BMP_HIGH();
        status = bmp280_read_raw(&bmp, &temp_raw, &press_raw); // HAL_OK / HAL_BUSY / HAL_ERROR
        MARK_BMP_LOW();
        if (status == HAL_TIMEOUT)
        {
            i2c_bus_recover(s_hi2c);
        }
        i2c_mutex_give();
        temperature = bmp280_compensate_temperature(&bmp, temp_raw);
        pressure = bmp280_compensate_pressure(&bmp, press_raw);
        altitude = bmp280_calculate_altitude(pressure, bmpDEFAULT_SEA_LEVEL);
    }

    return status;
}

float bmp_service_get_temperature(void)
{
    return temperature;
}
float bmp_service_get_pressure(void)
{
    return pressure;
}
float bmp_service_get_altitude(void)
{
    return altitude;
}