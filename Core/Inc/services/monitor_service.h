// Abstract layer for independence of gatekeeper task-writing into the monitor
#ifndef __MONITOR_SERVICE_H
#define __MONITOR_SERVICE_H
#include "ssd1306.h"
#include "fonts.h"

// For 128x64.
#define monitorDEFAULT_FONT (Font_7x10)
#define monitorCHAR_W 7
#define monitorCHAR_H 10
#define monitorCOLS 128 / monitorCHAR_W
#define monitorROWS 64 / monitorCHAR_H
#define monitorDEFAULT_COLOR White

#define monitorMAX_MUTEX_TIMEOUT_MS 80

void monitor_init(I2C_HandleTypeDef *bus);
void monitor_write_line(uint8_t line, const char *str);
#endif