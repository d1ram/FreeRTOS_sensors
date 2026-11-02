#include "FreeRTOS.h"
#include "monitor_service.h"
#include <stdio.h>

static I2C_HandleTypeDef *s_hi2c;

void monitor_init(I2C_HandleTypeDef *bus)
{
    s_hi2c = bus;
    ssd1306_Init(s_hi2c);
}

void monitor_write_line(uint8_t line, const char *str)
{
    char buf[monitorCOLS + 1];
    snprintf(buf, sizeof(buf), "%-*.*s", monitorCOLS, monitorCOLS, str);

    ssd1306_SetCursor(0, line * monitorCHAR_H);
    ssd1306_WriteString(buf, monitorDEFAULT_FONT, monitorDEFAULT_COLOR);
    ssd1306_UpdateScreen(s_hi2c);
}