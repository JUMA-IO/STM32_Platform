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
#include "bsp_isr.h"

extern RTC_HandleTypeDef RTCHandle;
volatile uint32_t ms_counter = 0;
extern SPI_HandleTypeDef SpiHandle;

void NMI_Handler(void)
{
}

void HardFault_Handler(void)
{
  while (1){}
}

void MemManage_Handler(void)
{
  while (1){}
}

void BusFault_Handler(void)
{
  while (1){}
}

void UsageFault_Handler(void)
{
  while (1){}
}

void SVC_Handler(void)
{
}

void DebugMon_Handler(void)
{
}

void PendSV_Handler(void)
{
}

void SysTick_Handler(void)
{
  HAL_IncTick();
}

void BNRG_SPI_EXTI_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(BNRG_SPI_EXTI_PIN);
}

extern ADC_HandleTypeDef    AdcHandle;
void ADC_IRQHandler(void)
{
  HAL_ADC_IRQHandler(&AdcHandle);
}

void DMA2_Stream0_IRQHandler(void)
{
  HAL_DMA_IRQHandler(AdcHandle.DMA_Handle);
}

