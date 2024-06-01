#include <stdio.h>

#include <main.h>

#define ARRAY_SIZE(__X__) (sizeof(__X__)/sizeof(*__X__))

static FILE* file(int fd) {
  static FILE* f[32] = { 0 };
  if(ARRAY_SIZE(f) < fd)
    return 0;
  if(0 == f[fd])
    f[fd] = fdopen(fd, "wr");
  return f[fd];
}

void test(void) {
  static int a = 0;
  if(((ITM->TCR & ITM_TCR_ITMENA_Msk) != 0UL) && ((ITM->TER/* & 1UL*/) != 0UL))
    HAL_GPIO_TogglePin(led0_GPIO_Port, led0_Pin);
  else
    HAL_GPIO_WritePin(led0_GPIO_Port, led0_Pin, GPIO_PIN_SET);
  HAL_GPIO_TogglePin(led1_GPIO_Port, led1_Pin);
  HAL_Delay(1000);
  fprintf(file(10), "SWO out to file: %d\n", a);
  printf("SWO out to STDOUT %d\n", a++);
}
