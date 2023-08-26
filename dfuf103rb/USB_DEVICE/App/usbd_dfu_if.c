/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : usbd_dfu_if.c
  * @brief          : Usb device for Download Firmware Update.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "usbd_dfu_if.h"

/* USER CODE BEGIN INCLUDE */

/* USER CODE END INCLUDE */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @brief Usb device.
  * @{
  */

/** @defgroup USBD_DFU
  * @brief Usb DFU device module.
  * @{
  */

/** @defgroup USBD_DFU_Private_TypesDefinitions
  * @brief Private types.
  * @{
  */

/* USER CODE BEGIN PRIVATE_TYPES */
#ifndef FLASH_DESC_STR
/* USER CODE END PRIVATE_TYPES */

/**
  * @}
  */

/** @defgroup USBD_DFU_Private_Defines
  * @brief Private defines.
  * @{
  */

#define FLASH_DESC_STR      "@Internal Flash   /0x08000000/48*1Ka,80*1Kg"

/* USER CODE BEGIN PRIVATE_DEFINES */
#endif /*FLASH_DESC_STR*/
#define FLASH_ERASE_TIME    (uint16_t)50
#define FLASH_PROGRAM_TIME  (uint16_t)50
/* USER CODE END PRIVATE_DEFINES */

/**
  * @}
  */

/** @defgroup USBD_DFU_Private_Macros
  * @brief Private macros.
  * @{
  */

/* USER CODE BEGIN PRIVATE_MACRO */

/* USER CODE END PRIVATE_MACRO */

/**
  * @}
  */

/** @defgroup USBD_DFU_Private_Variables
  * @brief Private variables.
  * @{
  */

/* USER CODE BEGIN PRIVATE_VARIABLES */

/* USER CODE END PRIVATE_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_DFU_Exported_Variables
  * @brief Public variables.
  * @{
  */

extern USBD_HandleTypeDef hUsbDeviceFS;

/* USER CODE BEGIN EXPORTED_VARIABLES */
/* USER CODE END EXPORTED_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_DFU_Private_FunctionPrototypes
  * @brief Private functions declaration.
  * @{
  */

static uint16_t MEM_If_Init_FS(void);
static uint16_t MEM_If_Erase_FS(uint32_t Add);
static uint16_t MEM_If_Write_FS(uint8_t *src, uint8_t *dest, uint32_t Len);
static uint8_t *MEM_If_Read_FS(uint8_t *src, uint8_t *dest, uint32_t Len);
static uint16_t MEM_If_DeInit_FS(void);
static uint16_t MEM_If_GetStatus_FS(uint32_t Add, uint8_t Cmd, uint8_t *buffer);

/* USER CODE BEGIN PRIVATE_FUNCTIONS_DECLARATION */
#ifdef UART_DEBUG
#ifdef UART_DEBUG_BUFFER
void printbuf(const uint8_t* buf, const uint32_t size) {
  for(uint32_t i = 0; i < size; ++i)
    printf("0x%02X%c", buf[i], ((i + 1) % 16 ? ' ' : '\n'));
}
#endif /*UART_DEBUG_BUFFER*/
#endif /*UART_DEBUG*/

/* USER CODE END PRIVATE_FUNCTIONS_DECLARATION */

/**
  * @}
  */

#if defined ( __ICCARM__ ) /* IAR Compiler */
  #pragma data_alignment=4
#endif
__ALIGN_BEGIN USBD_DFU_MediaTypeDef USBD_DFU_fops_FS __ALIGN_END =
{
   (uint8_t*)FLASH_DESC_STR,
    MEM_If_Init_FS,
    MEM_If_DeInit_FS,
    MEM_If_Erase_FS,
    MEM_If_Write_FS,
    MEM_If_Read_FS,
    MEM_If_GetStatus_FS
};

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Memory initialization routine.
  * @retval USBD_OK if operation is successful, MAL_FAIL else.
  */
uint16_t MEM_If_Init_FS(void)
{
  /* USER CODE BEGIN 0 */
#ifdef UART_DEBUG
  printf("%s\n", __FUNCTION__);
#endif /*UART_DEBUG*/
  HAL_StatusTypeDef flash_ok = HAL_ERROR;
  //Делаем память открытой
  while(flash_ok != HAL_OK)
    flash_ok = HAL_FLASH_Unlock();
  return (USBD_OK);
  /* USER CODE END 0 */
}

/**
  * @brief  De-Initializes Memory
  * @retval USBD_OK if operation is successful, MAL_FAIL else
  */
uint16_t MEM_If_DeInit_FS(void)
{
  /* USER CODE BEGIN 1 */
#ifdef UART_DEBUG
  printf("%s\n", __FUNCTION__);
#endif /*UART_DEBUG*/
  HAL_StatusTypeDef flash_ok = HAL_ERROR;
  //Закрываем память
  flash_ok = HAL_ERROR;
  while(flash_ok != HAL_OK)
    flash_ok = HAL_FLASH_Lock();
  return (USBD_OK);
  /* USER CODE END 1 */
}

/**
  * @brief  Erase sector.
  * @param  Add: Address of sector to be erased.
  * @retval 0 if operation is successful, MAL_FAIL else.
  */
uint16_t MEM_If_Erase_FS(uint32_t Add)
{
  /* USER CODE BEGIN 2 */
#ifdef UART_DEBUG
  printf("%s 0x%08X\n", __FUNCTION__, (unsigned int)Add);
#endif /*UART_DEBUG*/
  uint32_t PageError = 0;
/* Variable contains Flash operation status */
  FLASH_EraseInitTypeDef eraseinitstruct;

/* Get the number of sector to erase from 1st sector*/
  eraseinitstruct.TypeErase = FLASH_TYPEERASE_PAGES;
  eraseinitstruct.PageAddress = Add;
  eraseinitstruct.NbPages = 1;

  if (HAL_FLASHEx_Erase(&eraseinitstruct, &PageError) != HAL_OK)
    return (!USBD_OK);
  return (USBD_OK);
  /* USER CODE END 2 */
}

/**
  * @brief  Memory write routine.
  * @param  src: Pointer to the source buffer. Address to be written to.
  * @param  dest: Pointer to the destination buffer.
  * @param  Len: Number of data to be written (in bytes).
  * @retval USBD_OK if operation is successful, MAL_FAIL else.
  */
uint16_t MEM_If_Write_FS(uint8_t *src, uint8_t *dest, uint32_t Len)
{
  /* USER CODE BEGIN 3 */
#ifdef UART_DEBUG
  printf("%s src 0x%08X, dst 0x%08X, len 0x%08X\n", __FUNCTION__,
    (unsigned)src, (unsigned)dest, (unsigned)Len);
#ifdef UART_DEBUG_BUFFER
  printbuf(src, len);
#endif /*UART_DEBUG_BUFFER*/
#endif /*UART_DEBUG*/
  uint32_t i = 0;
  uint32_t* _dst = (uint32_t*)dest;
  uint32_t* _src = (uint32_t*)src;

  for(i = 0; i < Len / sizeof(uint32_t); ++i) {
    /* Device voltage range supposed to be [2.7V to 3.6V], the operation will be done by byte */
    if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, (uint32_t)(_dst++), (uint64_t)(*_src++)) != HAL_OK)
      /* Error occurred while writing data in Flash memory */
      return (USBD_BUSY);
  }
  for(i = 0; i < Len / sizeof(uint32_t); ++i)
    if(*--_src != *--_dst)
      /* Flash content doesn't match SRAM content */
      return (USBD_FAIL);
  return (USBD_OK);
  /* USER CODE END 3 */
}

/**
  * @brief  Memory read routine.
  * @param  src: Pointer to the source buffer. Address to be written to.
  * @param  dest: Pointer to the destination buffer.
  * @param  Len: Number of data to be read (in bytes).
  * @retval Pointer to the physical address where data should be read.
  */
uint8_t *MEM_If_Read_FS(uint8_t *src, uint8_t *dest, uint32_t Len)
{
  /* Return a valid address to avoid HardFault */
  /* USER CODE BEGIN 4 */
#ifdef UART_DEBUG
  printf("%s src 0x%08X, dst 0x%08X, len 0x%08X\n", __FUNCTION__,
    (unsigned)src, (unsigned)dest, (unsigned)Len);
#endif /*UART_DEBUG*/
  uint32_t i;
  for(i = 0; i < Len; i++)
    *dest++ = *src++;
  return (uint8_t*)(dest);
  /* USER CODE END 4 */
}

/**
  * @brief  Get status routine
  * @param  Add: Address to be read from
  * @param  Cmd: Number of data to be read (in bytes)
  * @param  buffer: used for returning the time necessary for a program or an erase operation
  * @retval USBD_OK if operation is successful
  */
uint16_t MEM_If_GetStatus_FS(uint32_t Add, uint8_t Cmd, uint8_t *buffer)
{
  /* USER CODE BEGIN 5 */
#ifdef UART_DEBUG
  printf("%s\n", __FUNCTION__);
#endif /*UART_DEBUG*/
  switch (Cmd) {
    case DFU_MEDIA_PROGRAM: {
      buffer[1] = (uint8_t)FLASH_PROGRAM_TIME;
      buffer[2] = (uint8_t)(FLASH_PROGRAM_TIME << 8);
      buffer[3] = 0;
      break;
    }
    case DFU_MEDIA_ERASE:
    default: {
      buffer[1] = (uint8_t)FLASH_ERASE_TIME;
      buffer[2] = (uint8_t)(FLASH_ERASE_TIME << 8);
      buffer[3] = 0;
      break;
    }
  }
  return  (USBD_OK);
  /* USER CODE END 5 */
}

/* USER CODE BEGIN PRIVATE_FUNCTIONS_IMPLEMENTATION */

/* USER CODE END PRIVATE_FUNCTIONS_IMPLEMENTATION */

/**
  * @}
  */

/**
  * @}
  */

