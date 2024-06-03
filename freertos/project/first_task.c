#include <stdio.h>
#include <stdint.h>

#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>

#include <main.h>

#include "tools.h"
#include "first_task.h"

#define __MAX_RECURSION__ 32
#define __STR_SIZE__ 32

static void recursion(uint32_t count, uint32_t* deep) {
  if(0 < count) {
    *deep = __get_PSP();
    recursion(--count, deep);
  }
}

static void show_stack_deep(QueueHandle_t queue, const uint32_t start,
    const uint32_t curdeep) {
  char* str = pvPortMalloc(__STR_SIZE__);
  if(0 != str) {
    str[snprintf(str, __STR_SIZE__ - 1, "first_task deep recursion %ld",
      (start - curdeep)/sizeof(void*))] = 0;
    if(!xQueueSend(queue, &str, 0))
      vPortFree(str);
  }
}

void first_task(void* param) {
  const uint32_t start = __get_PSP();
  uint32_t idx = 0;
  for(;;) {
    uint32_t deep = start;
    HAL_GPIO_TogglePin(led0_GPIO_Port, led0_Pin);
    recursion(idx++  % __MAX_RECURSION__, &deep);
    show_stack_deep((QueueHandle_t)param, start, deep);
    vTaskDelay(1000);
  }
}
