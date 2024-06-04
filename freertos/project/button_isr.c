#include "tools.h"
#include "imanager.h"

static void prepare_button_isr(const isr_data_t* value) {
  logout("button %d is pressed.", value->uint16);
}

void HAL_GPIO_EXTI_Callback(uint16_t value) {
  isr_decs_t desc;
  desc.data.uint16 = value;
  desc.isr_handler = prepare_button_isr;
  prepare_isr(&desc);
}
