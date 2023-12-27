
//#include "stm32f1xx_hal_def.h"
//#include "FreeRTOS.h"
#include "main.h"
#include "freertos.h"


/* USER CODE BEGIN PREPOSTSLEEP */
__attribute__((weak)) void PreSleepProcessing(TickType_t ulExpectedIdleTime) {
/* place for user code */
}

__attribute__((weak)) void PostSleepProcessing(TickType_t ulExpectedIdleTime) {
/* place for user code */
}
/* USER CODE END PREPOSTSLEEP */

void vApplicationMallocFailedHook(void) {
}

void vApplicationDaemonTaskStartupHook(void) {
}

void vApplicationIdleHook(void) {
}

void vApplicationTickHook(void) {
}

void vApplicationStackOverflowHook(TaskHandle_t xTask, char* pcTaskName) {
}

void vApplicationGetIdleTaskMemory(StaticTask_t** ppxIdleTaskTCBBuffer,
    StackType_t** ppxIdleTaskStackBuffer, uint32_t* pulIdleTaskStackSize) {
}

void vApplicationGetTimerTaskMemory(StaticTask_t** ppxTimerTaskTCBBuffer,
    StackType_t** ppxTimerTaskStackBuffer, uint32_t* pulTimerTaskStackSize ) {
}

static void thread1(void* parameters) {
  int a = 0;
  for(;;++a) {
    HAL_GPIO_WritePin(led0_GPIO_Port, led0_Pin, 0 == a % 2 ? GPIO_PIN_RESET : GPIO_PIN_SET);
    vTaskDelay(1000);
  }
}

static void thread2(void* parameters) {
  int a = 0;
  for(;;++a) {
    HAL_GPIO_WritePin(led1_GPIO_Port, led1_Pin, 0 == a % 2 ? GPIO_PIN_RESET : GPIO_PIN_SET);
    vTaskDelay(2000);
  }
}


tread_info_t tasks[] = {
  {&thread1, "thread1", 512, 0, 1, 0 },
  {&thread2, "thread2", 512, 0, 1, 0 }
};

BaseType_t init_system() {
  TaskHandle_t h;
  BaseType_t r = pdTRUE;
  for(size_t i = 0; i < sizeof(tasks)/sizeof(tread_info_t); ++i)
    if(pdFALSE == xTaskCreate(tasks[i].function, tasks[i].name, tasks[i].stackd_depth,
        tasks[i].parameters, tasks[i].priority, &h))
      r = pdFALSE;
  return r;
}