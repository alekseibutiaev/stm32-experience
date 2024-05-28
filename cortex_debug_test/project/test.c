
#include <main.h>


void test(void) {
  HAL_GPIO_TogglePin(led0_GPIO_Port, led0_Pin);
  HAL_GPIO_TogglePin(led1_GPIO_Port, led1_Pin);
  HAL_Delay(1000);
}