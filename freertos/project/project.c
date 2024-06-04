#include <stdint.h>
#include <stdio.h>

#include <stm32f1xx.h>

#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>

#include <main.h>

#include "first_task.h"
#include "second_task.h"
#include "imanager.h"

#include "tools.h"
#include "project.h"

/* Idle task control block and stack */
static StaticTask_t Idle_TCB;
static StackType_t  Idle_Stack[configMINIMAL_STACK_SIZE];

/* Timer task control block and stack */
static StaticTask_t Timer_TCB;
static StackType_t  Timer_Stack[configTIMER_TASK_STACK_DEPTH];

int __io_putchar(int ch) {
  // Write character to ITM ch.0
  ITM_SendChar(ch);
  return(ch);
}

void vApplicationStackOverflowHook(TaskHandle_t task, char* name) {
  return;
}

void vApplicationGetIdleTaskMemory(StaticTask_t** ppxIdleTaskTCBBuffer,
    StackType_t **ppxIdleTaskStackBuffer, uint32_t* pulIdleTaskStackSize) {
  *ppxIdleTaskTCBBuffer   = &Idle_TCB;
  *ppxIdleTaskStackBuffer = &Idle_Stack[0];
  *pulIdleTaskStackSize   = (uint32_t)configMINIMAL_STACK_SIZE;
}

/******************************************************************************/
/* vApplicationGetTimerTaskMemory gets called when                            */
/* configSUPPORT_STATIC_ALLOCATION                                            */
/* equals to 1 and is required for static memory allocation support.          */
/******************************************************************************/
void vApplicationGetTimerTaskMemory (StaticTask_t** ppxTimerTaskTCBBuffer,
    StackType_t** ppxTimerTaskStackBuffer, uint32_t* pulTimerTaskStackSize) {
  *ppxTimerTaskTCBBuffer   = &Timer_TCB;
  *ppxTimerTaskStackBuffer = &Timer_Stack[0];
  *pulTimerTaskStackSize   = (uint32_t)configTIMER_TASK_STACK_DEPTH;
}

void start(void) {
  xTaskCreate(first_task, "FIRST", 128 * sizeof(void*), 0, 1, 0);
  xTaskCreate(second_task, "SECOND", 128 * sizeof(void*), 0, 1, 0);
  xTaskCreate(ist_task, "IMG", 128 * sizeof(void*), 0, 1, 0);
  vTaskStartScheduler();
}
