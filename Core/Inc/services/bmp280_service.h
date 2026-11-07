#ifndef __BMP280_SERVICE_H
#define __BMP280_SERVICE_H
#include "BMP280.h"

#define bmpADDRESS 0x77
#define bmpMAX_MUTEX_TIMEOUT_MS 100
#define bmpDEFAULT_SEA_LEVEL 1000 // For altitude.

void bmp_service_init(I2C_HandleTypeDef *bus);

HAL_StatusTypeDef bmp_service_read_data(void);

float bmp_service_get_temperature(void);
float bmp_service_get_pressure(void);
float bmp_service_get_altitude(void);

#endif