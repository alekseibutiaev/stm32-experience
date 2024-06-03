#include <stdio.h>
#include <stdarg.h>

#include <FreeRTOS.h>
#include <queue.h>


#include "tools.h"

#define __STR_SIZE__ 64

char* print_log(const char* fmt, ...) {
  char* str = pvPortMalloc(__STR_SIZE__);
  if(0 != str) {
    va_list args;
    va_start(args, fmt);
    str[vsnprintf(str, __STR_SIZE__ - 1, fmt, args)] = 0;
    va_end(args);
  }
  return str;
}

void put_to_queue(void* queue, char* value) {
  if(0 != value && !xQueueSend((QueueHandle_t)queue, &value, 0))
    vPortFree(value);
}