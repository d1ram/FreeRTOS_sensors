// Mutex for I2C bus.
#ifndef __I2C_BUS_H
#define __I2C_BUS_H

#define SCL_PIN GPIO_PIN_8
#define SDA_PIN GPIO_PIN_9
#define half_period() vTaskDelay(pdMS_TO_TICKS(1))

#include "FreeRTOS.h"
#include "semphr.h"

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_i2c.h"

extern SemaphoreHandle_t i2c_mutex;

void i2c_mutex_init();
HAL_StatusTypeDef i2c_bus_recover(I2C_HandleTypeDef *hi2c);
BaseType_t i2c_mutex_take(TickType_t timeout); // pdTRUE if took, else pdFALSE.
void i2c_mutex_give();

#endif