/*
 *
 *  JUMA.IO - JUMA SDK for STM families
 *
 *  Copyright (C) 2013-2016  JUMA Technology
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the Apache V2 License as published by
 *  the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 */
#include "bsp_common.h"
#include "stm32_bluenrg_ble.h"
#include "bluenrg_sdk_api.h"
#include "bluenrg_sdk_host_api.h"
#include "dispatch.h"
#include "stm32f401_lp_mode.h"

#define BUFFER_SIZE         ((uint32_t)0x400)
#define WRITE_READ_ADDR     ((uint32_t)0x8000)
#define MANUFACTURER_CODE   ((uint16_t)0x0001)
#define DEVICE_CODE1        ((uint16_t)0x227E)
#define DEVICE_CODE2        ((uint16_t)0x2221)
#define DEVICE_CODE3        ((uint16_t)0x2201)  /*00h for PC28F128M29EWLA.*/

#define NOR_BANK_ADDR       ((uint32_t)0x60000000)
#define NOR_TIMEOUT_VALUE   ((uint32_t)0xFFFF)

NOR_HandleTypeDef hnor;
FMC_NORSRAM_TimingTypeDef NOR_Timing;
/* NOR IDs structure */
static NOR_IDTypeDef NOR_Id;

/* Read/Write Buffers */
uint16_t aTxBuffer[BUFFER_SIZE];
uint16_t aRxBuffer[BUFFER_SIZE];

/* Status variables */
__IO uint32_t uwWriteReadStatus = 0;

/* Counter index */
uint32_t uwIndex = 0;
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

static void Error_Handler(void);
static void Fill_Buffer(uint16_t *pBuffer, uint32_t uwBufferLength, uint16_t uwOffset);
static TestStatus Buffercmp(uint16_t *pBuffer1, uint16_t *pBuffer2, uint16_t BufferLength);

static void Error_Handler(void)
{
  while (1)
  {
  }
}

int FMC_Test(void)
{
  uint16_t *pdata = NULL;
  uint32_t index  = 0;
  uint32_t startaddress = 0;

  /*##-1- Configure the NOR device ##########################################*/
  /* NOR device configuration */

  hnor.Instance  = FMC_NORSRAM_DEVICE;
  hnor.Extended  = FMC_NORSRAM_EXTENDED_DEVICE;

  /* NOR device configuration */  
  NOR_Timing.AddressSetupTime       = 8;
  NOR_Timing.AddressHoldTime        = 3;
  NOR_Timing.DataSetupTime          = 9;
  NOR_Timing.BusTurnAroundDuration  = 0;
  NOR_Timing.CLKDivision            = 2;
  NOR_Timing.DataLatency            = 1;
  NOR_Timing.AccessMode             = FMC_ACCESS_MODE_B; 
	
	
  hnor.Init.NSBank                 = FMC_NORSRAM_BANK1;
  hnor.Init.DataAddressMux         = FMC_DATA_ADDRESS_MUX_ENABLE;
  hnor.Init.MemoryType             = FMC_MEMORY_TYPE_NOR;
  hnor.Init.MemoryDataWidth        = FMC_NORSRAM_MEM_BUS_WIDTH_16;
  hnor.Init.BurstAccessMode        = FMC_BURST_ACCESS_MODE_DISABLE;
  hnor.Init.WaitSignalPolarity     = FMC_WAIT_SIGNAL_POLARITY_LOW;
  hnor.Init.WrapMode               = FMC_WRAP_MODE_DISABLE;
  hnor.Init.WaitSignalActive       = FMC_WAIT_TIMING_BEFORE_WS;
  hnor.Init.WriteOperation         = FMC_WRITE_OPERATION_ENABLE;
  hnor.Init.WaitSignal             = FMC_WAIT_SIGNAL_DISABLE;
  hnor.Init.ExtendedMode           = FMC_EXTENDED_MODE_DISABLE;
  hnor.Init.AsynchronousWait       = FMC_ASYNCHRONOUS_WAIT_DISABLE;
  hnor.Init.WriteBurst             = FMC_WRITE_BURST_DISABLE;

  /* Initialize the NOR controller */
  if(HAL_NOR_Init(&hnor, &NOR_Timing, &NOR_Timing) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* Read NOR memory ID */
  if(HAL_NOR_Read_ID(&hnor, &NOR_Id) != HAL_OK)
  {
    /* NOR read ID Error */
    Error_Handler();
  }

   /* Test the NOR ID correctness */
  if((NOR_Id.Manufacturer_Code != (uint16_t)MANUFACTURER_CODE)/* ||
     (NOR_Id.Device_Code1 != (uint16_t)DEVICE_CODE1) ||
     (NOR_Id.Device_Code2 != (uint16_t)DEVICE_CODE2) ||
     (NOR_Id.Device_Code3 != (uint16_t)DEVICE_CODE3)*/)
  {
    /* NOR ID not correct */
    Error_Handler();
  }

	while (1)
		;
  /* Return to read mode */
  HAL_NOR_ReturnToReadMode(&hnor);
  
  /* Erase the NOR memory block to write on */
  HAL_NOR_Erase_Block(&hnor, WRITE_READ_ADDR, NOR_BANK_ADDR);
  
  /* Return the NOR memory status */  
  if(HAL_NOR_GetStatus(&hnor, NOR_BANK_ADDR, NOR_TIMEOUT_VALUE) != HAL_NOR_STATUS_SUCCESS)
  {
    /* Erase Error */
    Error_Handler();
  }

  /*##-2- NOR memory read/write access ######################################*/
  /* Fill the buffer to write */
  Fill_Buffer(aTxBuffer, BUFFER_SIZE, 0xC20F);

  /* Write data to the NOR memory */
  pdata = aTxBuffer;
  index = BUFFER_SIZE;
  startaddress = NOR_BANK_ADDR + WRITE_READ_ADDR;
  while(index > 0)
  {
    /* Write data to NOR */
    HAL_NOR_Program(&hnor, (uint32_t *)startaddress, pdata);
    
    /* Read NOR device status */
    if(HAL_NOR_GetStatus(&hnor, NOR_BANK_ADDR, NOR_TIMEOUT_VALUE) != HAL_NOR_STATUS_SUCCESS)
    {
      Error_Handler();
    }
    
    /* Update the counters */
    index--;
    startaddress += 2;
    pdata++; 
  }

  /* Read back data from the NOR memory */
  if(HAL_NOR_ReadBuffer(&hnor, NOR_BANK_ADDR + WRITE_READ_ADDR, aRxBuffer, BUFFER_SIZE) != HAL_OK)
  {
      Error_Handler();
  }

  /*##-3- Checking data integrity ############################################*/
  uwWriteReadStatus = Buffercmp(aTxBuffer, aRxBuffer, BUFFER_SIZE);

  if(uwWriteReadStatus != PASSED)
  {
  }
  else
  {
  }

  /* Infinite loop */
  while (1)
  {
  }
}


/**
  * @brief  Fills buffer with user predefined data.
  * @param  pBuffer: pointer on the buffer to fill
  * @param  uwBufferLength: size of the buffer to fill
  * @param  uwOffset: first value to fill on the buffer
  * @retval None
  */
static void Fill_Buffer(uint16_t *pBuffer, uint32_t uwBufferLength, uint16_t uwOffset)
{
  uint16_t tmpIndex = 0;

  /* Put in global buffer different values */
  for (tmpIndex = 0; tmpIndex < uwBufferLength; tmpIndex++)
  {
    pBuffer[tmpIndex] = tmpIndex + uwOffset;
  }
}

/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval PASSED: pBuffer identical to pBuffer1
  *         FAILED: pBuffer differs from pBuffer1
  */
static TestStatus Buffercmp(uint16_t *pBuffer1, uint16_t *pBuffer2, uint16_t BufferLength)
{
  while (BufferLength--)
  {
    if (*pBuffer1 != *pBuffer2)
    {
      return FAILED;
    }

    pBuffer1++;
    pBuffer2++;
  }

  return PASSED;
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @brief NOR MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  * @param hsram: NOR handle pointer
  * @retval None
  */
void HAL_NOR_MspInit(NOR_HandleTypeDef *hnor)
{
  GPIO_InitTypeDef GPIO_Init_Structure;

  /* Enable FMC clock */
  __HAL_RCC_FMC_CLK_ENABLE();

  /* Enable GPIOs clock */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
//  __HAL_RCC_GPIOF_CLK_ENABLE();
//  __HAL_RCC_GPIOG_CLK_ENABLE();

  /* Common GPIO configuration */
  GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init_Structure.Pull      = GPIO_PULLUP;
  GPIO_Init_Structure.Speed     = GPIO_SPEED_HIGH;
  GPIO_Init_Structure.Alternate = GPIO_AF12_FMC;

  /*## Data Bus #######*/
  /* GPIOD configuration */
  GPIO_Init_Structure.Pin   = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_8 | GPIO_PIN_9 | \
                              GPIO_PIN_10 | GPIO_PIN_14 | GPIO_PIN_15;
  HAL_GPIO_Init(GPIOD, &GPIO_Init_Structure);

  /* GPIOE configuration */  
  GPIO_Init_Structure.Pin   = GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | \
                              GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | \
                              GPIO_PIN_14 | GPIO_PIN_15;
  HAL_GPIO_Init(GPIOE, &GPIO_Init_Structure);
  
  /*## Address Bus #######*/
//  /* GPIOF configuration */  
//  GPIO_Init_Structure.Pin   = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | \
//                              GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_12 | GPIO_PIN_13 | \
//                              GPIO_PIN_14 | GPIO_PIN_15;
//  HAL_GPIO_Init(GPIOF, &GPIO_Init_Structure);
//  
//  /* GPIOG configuration */  
//  GPIO_Init_Structure.Pin   = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | \
//                              GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5;
//  HAL_GPIO_Init(GPIOG, &GPIO_Init_Structure);
  
  /* GPIOD configuration */
  GPIO_Init_Structure.Pin   = GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13;
  HAL_GPIO_Init(GPIOD, &GPIO_Init_Structure);

  /* GPIOE configuration */  
  GPIO_Init_Structure.Pin   = GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6;
  HAL_GPIO_Init(GPIOE, &GPIO_Init_Structure);

  /*## NOE and NWE configuration #######*/ 
  GPIO_Init_Structure.Pin = GPIO_PIN_4 |GPIO_PIN_5;
  HAL_GPIO_Init(GPIOD, &GPIO_Init_Structure);

  /*## WAIT and CLK????????? configuration #######*/ 
  GPIO_Init_Structure.Pin = GPIO_PIN_6 |GPIO_PIN_3;
  HAL_GPIO_Init(GPIOD, &GPIO_Init_Structure);

  /*## NL pin configuration #######*/
  GPIO_Init_Structure.Pin = GPIO_PIN_7;
  HAL_GPIO_Init(GPIOB, &GPIO_Init_Structure);
	
	
	
  /*## Enable Bank pin configuration #######*/
  GPIO_Init_Structure.Pin = GPIO_PIN_7;
  HAL_GPIO_Init(GPIOD, &GPIO_Init_Structure);

  /*## NBL0, NBL1 configuration #######*/
  GPIO_Init_Structure.Pin = GPIO_PIN_0 | GPIO_PIN_1;
  HAL_GPIO_Init(GPIOE, &GPIO_Init_Structure); 
}

/**
  * @brief NOR MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO configuration to their default state
  * @param hnor: NOR handle pointer
  * @retval None
  */
void HAL_NOR_MspDeInit(NOR_HandleTypeDef *hnor)
{
//  /*## Disable peripherals and GPIO Clocks ###################################*/
//  /* Configure FMC as alternate function  */
//  HAL_GPIO_DeInit(GPIOD, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5     | \
//                  GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | \
//                  GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);

//  HAL_GPIO_DeInit(GPIOE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_7     | \
//                  GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | \
//                  GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);

//  HAL_GPIO_DeInit(GPIOF, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | \
//                  GPIO_PIN_5 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);

//  HAL_GPIO_DeInit(GPIOG, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | \
//                  GPIO_PIN_5 | GPIO_PIN_9);
}
