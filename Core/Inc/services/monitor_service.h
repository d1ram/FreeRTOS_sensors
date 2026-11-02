#include "ssd1306.h"
#include "fonts.h"
#include <stdio.h>

#define __MONITOR_SERVICE_H
#ifdef __MONITOR_SERVICE_H

// For 128x64.
#define monitorDEFAULT_FONT (Font_7x10)
#define monitorCHAR_W 7
#define monitorCHAR_H 10
#define monitorCOLS 128 / monitorCHAR_W
#define monitorROWS 64 / monitorCHAR_H
#define monitorDEFAULT_COLOR White

void monitor_init(I2C_HandleTypeDef *bus);
void monitor_write_line(uint8_t line, const char *str);
#endif