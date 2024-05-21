/*
 * function.c
 *
 *  Created on: May 20, 2024
 *      Author: aleksei
 */

#include "main.h"
#include "spi.h"
#include "gpio.h"

#define array_size(__a__) sizeof(__a__)/sizeof(*__a__)

uint8_t buf[] = {0x80, 0xAA, 0xFF, 0x55, 0x00, 0x01, 0x00};

void function() {

	HAL_GPIO_WritePin(led_GPIO_Port, led_Pin, GPIO_PIN_SET);
  HAL_Delay(1);
	HAL_SPI_Transmit_DMA(&hspi1, buf, array_size(buf));
 // HAL_Delay(1);
}


void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef* hspi) {
//	HAL_GPIO_TogglePin(led_GPIO_Port, led_Pin);
//	a = 0;
	hspi->Instance->DR = 0;
  HAL_GPIO_WritePin(led_GPIO_Port, led_Pin, GPIO_PIN_RESET);
	return;
}

void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi) {

}


void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi) {

}

void HAL_SPI_TxHalfCpltCallback(SPI_HandleTypeDef *hspi) {
//	HAL_GPIO_TogglePin(led_GPIO_Port, led_Pin);
//	a = 0;
	return;
}

void HAL_SPI_RxHalfCpltCallback(SPI_HandleTypeDef *hspi) {

}

void HAL_SPI_TxRxHalfCpltCallback(SPI_HandleTypeDef *hspi) {

}


void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *tim_handle) {
	return;
}
