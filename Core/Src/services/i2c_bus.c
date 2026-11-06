#include "i2c_bus.h"

#include "FreeRTOS.h"
#include "semphr.h"

SemaphoreHandle_t i2c_mutex = NULL;

void i2c_mutex_init()
{
    i2c_mutex = xSemaphoreCreateMutex();
    configASSERT(i2c_mutex != NULL);
}
BaseType_t i2c_mutex_take(TickType_t timeout)
{
    return xSemaphoreTake(i2c_mutex, timeout);
}
void i2c_mutex_give()
{
    xSemaphoreGive(i2c_mutex);
}