// Watchdog service that provides task with every needed.
#ifndef __WATCHDOG_SERVICE_H
#define __WATCHDOG_SERVICE_H

#include <stdint.h>
#include <stdbool.h>

#define watchdogKEY_START 0xCCCC         // Start timer
#define watchdogKEY_REFRESH 0xAAAA       // Refresh timer
#define watchdogKEY_PR_RLP_ACCESS 0x5555 // Access to PR / RLP

#define watchdogMAX_TASKS 16 // Max amount of tasks that watchdog can process.
#define watchdogFAULT_TASK_MAX_NAME 32

typedef enum
{
    OK = 0,
    SENSOR_ERROR,
    BUS_ERROR
} WatchdogTaskStatus_t;

struct
{
    uint32_t task_id;
    uint32_t min_time; // Protection from spam.
    uint32_t max_time;
    volatile uint32_t last_ping;
    volatile bool has_pinged; // If ever pinged (protection from born and never pinged).
    volatile bool is_active;
    char *task_name;
    WatchdogTaskStatus_t status;
} typedef WatchdogTask_t;

typedef struct
{
    int32_t task_watchdog_id;
    int32_t task_gatekeeper_id;
} WatchdogTaskNotify_t;

void watchdog_init(void);
int32_t watchdog_add_task(uint32_t min_time, uint32_t max_time, char *task_name);
void watchdog_ping(uint32_t task_id, uint32_t ping_time, WatchdogTaskStatus_t status);

const WatchdogTask_t *watchdog_get_task(uint32_t task_id);
uint32_t watchdog_get_counter(void);

void watchdog_refresh_timer(void);

#endif