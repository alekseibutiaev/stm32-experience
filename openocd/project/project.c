#include <gpio.h>
#include <main.h>

#include "project.h"


void test() {
  static union {
    struct {
      int a : 1;
      int   : 3;
      int b : 1;
    } bit;
    unsigned char value;
  } status;
  HAL_GPIO_WritePin(led1_GPIO_Port, led1_Pin, status.bit.a ? GPIO_PIN_RESET : GPIO_PIN_SET);
  HAL_GPIO_WritePin(led0_GPIO_Port, led0_Pin, status.bit.b ? GPIO_PIN_RESET : GPIO_PIN_SET);
  ++status.value;
}