#ifndef __IMANAGER_H__
#define __IMANAGER_H__

#include <stdint.h>

typedef union {
  int8_t int8;
  uint8_t uint8;
  int16_t int16;
  uint16_t uint16;
  int32_t int32;
  uint32_t uint32;
  int64_t ini64;
  uint64_t uint64;
  void* ptr;
} isr_data_t;

typedef void(*isr_handler_t)(const isr_data_t*);

typedef struct {
  isr_data_t data;
  isr_handler_t isr_handler;
} isr_decs_t;

void ist_task(void* param);

void prepare_isr(const isr_decs_t* isr_decs);

void prepare(const isr_decs_t* isr_decs);


#endif /* __IMANAGER_H__ */