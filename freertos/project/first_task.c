#include <stdio.h>
#include <stdint.h>

#include <FreeRTOS.h>
#include <task.h>

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

void first_task(void* param) {
  const uint32_t start = __get_PSP();
  uint32_t idx = 0;
  for(;;) {
    uint32_t deep = start;
    HAL_GPIO_TogglePin(led0_GPIO_Port, led0_Pin);
    recursion(idx++  % __MAX_RECURSION__, &deep);
    logout("first_task deep recursion %ld", (start - deep)/sizeof(void*));
    vTaskDelay(1000);
  }
}
