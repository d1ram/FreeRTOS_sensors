## i2c_bus.c <- i2c_bus_recover()

### Overview

`i2c_bus.c` now contains `HAL_StatusTypeDef i2c_bus_recover(I2C_HandleTypeDef *hi2c)`, which is a function for I2C-bus recover. Main idea: if I2C-bus is stuck or fell, mutex-holder will call `i2c_bus_recover` and it will unblock it.

### Why?

Sometimes situation can occur, in which plate will think, that I2C-bus is sending data at the moment (`BUSY`), but in reality I2C-bus itself will be waiting for signal from plate. That situation can stuck forever. For that case, i created a function that will recover bus from it's dead condition without hard-reset of the plate.

### How?

```
HAL_StatusTypeDef i2c_bus_recover(I2C_HandleTypeDef *hi2c)
{
    // At the start we will deinitialize our bus.
    HAL_I2C_DeInit(hi2c);

    GPIO_InitTypeDef g = {0};
    g.Pin = SCL_PIN | SDA_PIN;
    g.Mode = GPIO_MODE_OUTPUT_OD;
    g.Pull = GPIO_PULLUP;
    g.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &g); // reinitializing for access of hand-writing inside of register.

    HAL_GPIO_WritePin(GPIOB, SCL_PIN | SDA_PIN, GPIO_PIN_SET); // writing 1 inside of both registers for needed start point (1)
    half_period(); // vTaskDelay(1 tick)

    for (int i = 0; i < 9 && HAL_GPIO_ReadPin(GPIOB, SDA_PIN) == GPIO_PIN_RESET; i++) // now we need to write 9 bits inside of SCL pin
                                                                                         in order to fully send 1 byte of information (9 bit is ACL)
    {
        HAL_GPIO_WritePin(GPIOB, SCL_PIN, GPIO_PIN_RESET);
        half_period();
        HAL_GPIO_WritePin(GPIOB, SCL_PIN, GPIO_PIN_SET);
        half_period();
    }

    // STOP-condition. SDA goes down (0), SCL goes up (1) and SDA goes up (1). This is a STOP signal.
    HAL_GPIO_WritePin(GPIOB, SDA_PIN, GPIO_PIN_RESET);
    half_period();
    HAL_GPIO_WritePin(GPIOB, SCL_PIN, GPIO_PIN_SET);
    half_period();
    HAL_GPIO_WritePin(GPIOB, SDA_PIN, GPIO_PIN_SET);
    half_period();

    HAL_StatusTypeDef res = (HAL_GPIO_ReadPin(GPIOB, SDA_PIN) == GPIO_PIN_SET) ? HAL_OK : HAL_ERROR; // if SDA at the end is 1, bus can now
                                                                                                        again work correctly. If not, return ERROR.
    HAL_I2C_Init(hi2c);
    return res;
}
```
