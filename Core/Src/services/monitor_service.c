#include "monitor_service.h"

#include "i2c_bus.h"

#include "FreeRTOS.h"
#include "semphr.h"

#include "jitter_test.h"

#include <stdio.h>

static I2C_HandleTypeDef *s_hi2c;

void monitor_init(I2C_HandleTypeDef *bus)
{
    s_hi2c = bus;
    ssd1306_Init(s_hi2c);
}

static char buf[monitorCOLS + 1];
void monitor_set_line(uint8_t line, const char *str)
{
    if (i2c_mutex == NULL || s_hi2c == NULL)
        return;

    snprintf(buf, sizeof(buf), "%-*.*s", monitorCOLS, monitorCOLS, str);
    ssd1306_SetCursor(0, line * monitorCHAR_H);
    ssd1306_WriteString(buf, monitorDEFAULT_FONT, monitorDEFAULT_COLOR);
}
HAL_StatusTypeDef monitor_flush(void)
{
    if (i2c_mutex == NULL || s_hi2c == NULL)
        return HAL_ERROR;

    HAL_StatusTypeDef status = HAL_OK;

    if (i2c_mutex_take(pdMS_TO_TICKS(monitorMAX_MUTEX_TIMEOUT_MS)) == pdTRUE)
    { // Mutex for i2c bus
#ifdef DIAG
        MARK_BUS_HIGH();
#endif

        status = ssd1306_UpdateScreen(s_hi2c);

#ifdef DIAG
        MARK_BUS_LOW();
#endif

        if (status == HAL_ERROR)
        {
            uint32_t err_code = HAL_I2C_GetError(s_hi2c);

            if (err_code & HAL_I2C_ERROR_TIMEOUT)
            {
                status = HAL_TIMEOUT;
            }
        }

        if (status == HAL_TIMEOUT)
        {
            i2c_bus_recover(s_hi2c);
        }
        i2c_mutex_give();
    }
    else
    {
        status = HAL_TIMEOUT;
    }

    return status;
}
