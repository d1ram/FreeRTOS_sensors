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

void monitor_write_line(uint8_t line, const char *str)
{
    if (i2c_mutex == NULL || s_hi2c == NULL)
        return;

    char buf[monitorCOLS + 1];
    snprintf(buf, sizeof(buf), "%-*.*s", monitorCOLS, monitorCOLS, str);

    ssd1306_SetCursor(0, line * monitorCHAR_H);
    ssd1306_WriteString(buf, monitorDEFAULT_FONT, monitorDEFAULT_COLOR);

    if (i2c_mutex_take(pdMS_TO_TICKS(monitorMAX_MUTEX_TIMEOUT_MS)) == pdTRUE)
    { // Mutex for i2c bus
        // MARK_BUS_HIGH();
        ssd1306_UpdateScreen(s_hi2c);
        // MARK_BUS_LOW();
        i2c_mutex_give();
    }
}
