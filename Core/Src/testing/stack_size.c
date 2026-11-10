#include "stack_size.h"
#include "FreeRTOS.h"

#ifdef DIAG
volatile UBaseType_t g_stack_free[12];
#endif