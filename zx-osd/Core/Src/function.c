/*
 * function.c
 *
 *  Created on: May 20, 2024
 *      Author: aleksei
 */

#include "main.h"
#include "spi.h"
#include "gpio.h"

uint8_t buf[] = {0xAA, 0xFF, 0x55, 0x00};
static int a = 0;

void function() {

	HAL_GPIO_TogglePin(led_GPIO_Port, led_Pin);
  HAL_SPI_Transmit_DMA(&hspi1, buf, 4);
  for(a = 0; a < 32; ++a) {
    HAL_GPIO_TogglePin(spi_clk_GPIO_Port, spi_clk_Pin);
    HAL_GPIO_TogglePin(spi_clk_GPIO_Port, spi_clk_Pin);
  }
  HAL_Delay(10);
}


void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi) {

}

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi) {
	return;
}

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi) {

}

void HAL_SPI_TxHalfCpltCallback(SPI_HandleTypeDef *hspi) {
	return;
}

void HAL_SPI_RxHalfCpltCallback(SPI_HandleTypeDef *hspi) {

}

void HAL_SPI_TxRxHalfCpltCallback(SPI_HandleTypeDef *hspi) {

}


void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *tim_handle) {
	return;
}
