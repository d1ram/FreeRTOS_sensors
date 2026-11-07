#ifndef __FAULT_DIAG_H
#define __FAULT_DIAG_H

#define fault_diagFAULT_TASK_MAX_NAME 32
extern char fault_task_name[fault_diagFAULT_TASK_MAX_NAME];

typedef enum
{
    RESET_CAUSE_UNKNOWN = 0,
    RESET_CAUSE_IWDG,
    RESET_CAUSE_PIN,
    RESET_CAUSE_POWER_ON
} ResetCause_t;

typedef struct
{
    ResetCause_t cause;
    char faulty_task[fault_diagFAULT_TASK_MAX_NAME];
} SystemResetInfo_t;

void fault_diag_init(void); // Checks Reset cause, fills SystemResetInfo and clears .noinit
const SystemResetInfo_t *fault_get_info(void);

#endif