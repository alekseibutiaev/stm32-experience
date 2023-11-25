#include <stdint.h>
#include <main.h>
#include <project.h>


typedef union {
	struct {
		int a :1;
		int   :3;
		int b :1;
	} bits;
	uint8_t value;
} status_t;

static status_t st;

void init() {
	st.value = 0;
}

void process() {
	HAL_GPIO_WritePin(led0_GPIO_Port, led0_Pin, st.bits.a ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(led1_GPIO_Port, led1_Pin, st.bits.b ? GPIO_PIN_SET : GPIO_PIN_RESET);
	++st.value;
}
