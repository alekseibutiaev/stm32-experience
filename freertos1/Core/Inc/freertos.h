
#ifndef __FREERTOS_H__
#define __FREERTOS_H__

#include "FreeRTOS.h"
#include "task.h"

#if defined(__ICCARM__) || defined(__CC_ARM) || defined(__GNUC__)
//#if ( configUSE_16_BIT_TICKS == 1 )
//void PreSleepProcessing(uint16_t ulExpectedIdleTime);
//void PostSleepProcessing(uint16_t ulExpectedIdleTime);
//#else
void PreSleepProcessing(TickType_t ulExpectedIdleTime);
void PostSleepProcessing(TickType_t ulExpectedIdleTime);
//#endif
#endif /* defined(__ICCARM__) || defined(__CC_ARM) || defined(__GNUC__) */

typedef struct {
  TaskFunction_t function;
  const char* const name;
  const configSTACK_DEPTH_TYPE stackd_depth;
  void* const parameters;
  UBaseType_t priority;
  TaskHandle_t* handle;
} tread_info_t;

BaseType_t init_system();

#endif /* __FREERTOS_H__ */