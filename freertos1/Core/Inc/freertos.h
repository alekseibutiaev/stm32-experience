
#ifndef __FREERTOS_H__
#define __FREERTOS_H__

#include "FreeRTOS.h"

#if defined(__ICCARM__) || defined(__CC_ARM) || defined(__GNUC__)
//#if ( configUSE_16_BIT_TICKS == 1 )
//void PreSleepProcessing(uint16_t ulExpectedIdleTime);
//void PostSleepProcessing(uint16_t ulExpectedIdleTime);
//#else
void PreSleepProcessing(TickType_t ulExpectedIdleTime);
void PostSleepProcessing(TickType_t ulExpectedIdleTime);
//#endif
#endif /* defined(__ICCARM__) || defined(__CC_ARM) || defined(__GNUC__) */


#endif /* __FREERTOS_H__ */