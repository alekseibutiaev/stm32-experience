#include <stdio.h>
#include <stdarg.h>

#include <FreeRTOS.h>
#include <queue.h>


#include "tools.h"

#define __STR_SIZE__ 64

static QueueHandle_t msgq = 0;

void vApplicationIdleHook(void) {
  char* buf = 0;
  if(0 != xQueueReceive(msgq, &buf, (TickType_t)0)) {
    puts(buf);
    vPortFree(buf);
  }
}

void logout(const char* fmt, ...) {
  if(0 == msgq) {
    if(0 == (msgq = xQueueCreate(10, sizeof(void*))))
    return;
  }
  char* str = pvPortMalloc(__STR_SIZE__);
  if(0 != str) {
    va_list args;
    va_start(args, fmt);
    str[vsnprintf(str, __STR_SIZE__ - 1, fmt, args)] = 0;
    va_end(args);
    if(!xQueueSend(msgq, &str, 0))
      vPortFree(str);
  }
}
