#include <stm32f1xx.h>

#include <stdio.h>

#include <main.h>

#if 1
int __io_putchar(int ch) {
  ITM_SendChar(ch);
  return ch;
}
#endif

void test(void) {
  static int a = 0;
  HAL_GPIO_TogglePin(led0_GPIO_Port, led0_Pin);
  HAL_GPIO_TogglePin(led1_GPIO_Port, led1_Pin);
  HAL_Delay(1000);
  printf("SWO out %d\n", a++);
}