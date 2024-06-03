#include <stdint.h>
#include <stdio.h>

#include <FreeRTOS.h>
#include <task.h>

#include <main.h>

#include "tools.h"
#include "second_task.h"

void second_task(void* param) {
  int i = 0;
  for(;;) {
    HAL_GPIO_TogglePin(led1_GPIO_Port, led1_Pin);
    vTaskDelay(500);
    put_to_queue(param, print_log("second_task iteration %ld", i++));

    //printf("out %d\n", i++);
  }
}
