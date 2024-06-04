#include <FreeRTOS.h>
#include <queue.h>


#include "imanager.h"

static QueueHandle_t queue = 0;

void ist_task(void* param) {
  queue = xQueueCreate(10, sizeof(isr_decs_t));
  while(0 != queue) {
    isr_decs_t desc;
    xQueueReceive(queue, &desc, portMAX_DELAY);
    desc.isr_handler(&desc.data);
  }
}

void prepare_isr(const isr_decs_t* isr_decs) {
  BaseType_t xHigherPriorityTaskWoken = pdFALSE /*pdTRUE*/ ;
  if(0 != queue)
    xQueueSendFromISR(queue, isr_decs, &xHigherPriorityTaskWoken);
  return;
}

void prepare(const isr_decs_t* isr_decs){
  if(0 != queue)
    xQueueSend(queue, isr_decs, 0);
}

