
#include "i2c_bus.h"

#include "FreeRTOS.h"
#include "semphr.h"

SemaphoreHandle_t i2c_mutex = NULL;

void i2c_mutex_init()
{
    i2c_mutex = xSemaphoreCreateMutex();
    configASSERT(i2c_mutex != NULL);
}
HAL_StatusTypeDef i2c_bus_recover(I2C_HandleTypeDef *hi2c)
{
    HAL_I2C_DeInit(hi2c);

    GPIO_InitTypeDef g = {0};
    g.Pin = SCL_PIN | SDA_PIN;
    g.Mode = GPIO_MODE_OUTPUT_OD;
    g.Pull = GPIO_PULLUP;
    g.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &g);

    HAL_GPIO_WritePin(GPIOB, SCL_PIN | SDA_PIN, GPIO_PIN_SET);
    half_period();

    for (int i = 0; i < 9 && HAL_GPIO_ReadPin(GPIOB, SDA_PIN) == GPIO_PIN_RESET; i++)
    {
        HAL_GPIO_WritePin(GPIOB, SCL_PIN, GPIO_PIN_RESET);
        half_period();
        HAL_GPIO_WritePin(GPIOB, SCL_PIN, GPIO_PIN_SET);
        half_period();
    }

    HAL_GPIO_WritePin(GPIOB, SDA_PIN, GPIO_PIN_RESET);
    half_period();
    HAL_GPIO_WritePin(GPIOB, SCL_PIN, GPIO_PIN_SET);
    half_period();
    HAL_GPIO_WritePin(GPIOB, SDA_PIN, GPIO_PIN_SET);
    half_period();

    HAL_StatusTypeDef res = (HAL_GPIO_ReadPin(GPIOB, SDA_PIN) == GPIO_PIN_SET) ? HAL_OK : HAL_ERROR;
    HAL_I2C_Init(hi2c);
    return res;
}
BaseType_t i2c_mutex_take(TickType_t timeout)
{
    return xSemaphoreTake(i2c_mutex, timeout);
}
void i2c_mutex_give()
{
    xSemaphoreGive(i2c_mutex);
}