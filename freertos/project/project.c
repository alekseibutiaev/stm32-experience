#include <stdint.h>
#include <stdio.h>

#include <stm32f1xx.h>

#include <FreeRTOS.h>
#include <task.h>

#include <main.h>

#include "project.h"

/* Idle task control block and stack */
static StaticTask_t Idle_TCB;
static StackType_t  Idle_Stack[configMINIMAL_STACK_SIZE];

/* Timer task control block and stack */
static StaticTask_t Timer_TCB;
static StackType_t  Timer_Stack[configTIMER_TASK_STACK_DEPTH];

#if 0
int __io_putchar(int ch) {
  // Write character to ITM ch.0
  ITM_SendChar(ch);
  return(ch);
}
#endif

void vApplicationStackOverflowHook(TaskHandle_t task, char* name) {
  return;
}

void vApplicationGetIdleTaskMemory (StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize) {
  *ppxIdleTaskTCBBuffer   = &Idle_TCB;
  *ppxIdleTaskStackBuffer = &Idle_Stack[0];
  *pulIdleTaskStackSize   = (uint32_t)configMINIMAL_STACK_SIZE;
}

/*
  vApplicationGetTimerTaskMemory gets called when configSUPPORT_STATIC_ALLOCATION
  equals to 1 and is required for static memory allocation support.
*/
void vApplicationGetTimerTaskMemory (StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize) {
  *ppxTimerTaskTCBBuffer   = &Timer_TCB;
  *ppxTimerTaskStackBuffer = &Timer_Stack[0];
  *pulTimerTaskStackSize   = (uint32_t)configTIMER_TASK_STACK_DEPTH;
}

void recursion(uint32_t* r, UBaseType_t* deep) {
  if(0 < *r) {
    *deep = uxTaskGetStackHighWaterMark(0);
    --*r;
    recursion(r, deep);
  }
}

void task_one(void* param) {
  UBaseType_t start = uxTaskGetStackHighWaterMark(0);
  uint32_t buf[32] = { 0 };
  uint32_t i = 0;
  uint32_t r = 0;
  UBaseType_t deep = start - uxTaskGetStackHighWaterMark(0);
  for(;;) {
    HAL_GPIO_TogglePin(led0_GPIO_Port, led0_Pin);
    vTaskDelay(1000);
    ++buf[i++];
    r = i = i % ARRAY_SIZE(buf);
    recursion(&r, &deep);
    deep = start - deep;
  }
  (void)deep;
}

void task_two(void* param) {
  int i = 0;
  for(;;) {
    HAL_GPIO_TogglePin(led1_GPIO_Port, led1_Pin);
    vTaskDelay(500);
    printf("out %d\n", i++);
  }
}

void start(void) {
  xTaskCreate(task_one, "ONE", 128 * sizeof(void*), 0, 1, 0);
  xTaskCreate(task_two, "TWO", 128 * sizeof(void*), 0, 1, 0);
  vTaskStartScheduler();
}
