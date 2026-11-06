// Mutex for I2C bus.
#ifndef __I2C_BUS_H
#define __I2C_BUS_H

#include "FreeRTOS.h"
#include "semphr.h"

extern SemaphoreHandle_t i2c_mutex;

void i2c_mutex_init();
BaseType_t i2c_mutex_take(TickType_t timeout); // pdTRUE if took, else pdFALSE.
void i2c_mutex_give();

#endif