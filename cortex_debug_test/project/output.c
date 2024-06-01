#include <stm32f1xx.h>

#include "output.h"

#define ARRAY_SIZE(__X__) (sizeof(__X__)/sizeof(*__X__))

int __io_putchar(int ch) {
  ITM_SendChar(ch);
  return ch;
}

int put_char(int ch, int value) {
  if(ARRAY_SIZE(ITM->PORT) < ch)
    return 0;
  if(((ITM->TCR & ITM_TCR_ITMENA_Msk) != 0UL) &&  /* ITM enabled */
       ((ITM->TER & 1 << ch) != 0UL)) { /* ITM Port #0 enabled */
    while(ITM->PORT[ch].u32 == 0UL)
      __NOP();
    ITM->PORT[ch].u8 = (uint8_t)value;
  }
  return (value);
}

int _read(int file, char *ptr, int len) {
  return len;
}

int _write(int file, char* ptr, int len) {
  const int count = len;
  while(0 < len--)
    put_char(file, *ptr++);
  return count;
}
