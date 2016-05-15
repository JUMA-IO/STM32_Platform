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
#include "x_nucleo_iks01a1.h"

volatile uint32_t ms_counter = 0;
volatile uint8_t button_event = 0;

extern RTC_HandleTypeDef RTCHandle;
extern I2C_HandleTypeDef I2C_EXPBD_Handle;
extern SPI_HandleTypeDef SpiHandle;

/*NMI_Handler This function handles NMI exception.*/
void NMI_Handler(void)
{
}

/*HardFault_Handler This function handles Hard Fault exception.*/
void HardFault_Handler(void)
{
    while (1){}
}

/*SVC_Handler This function handles SVCall exception.*/
void SVC_Handler(void)
{
}

/*DebugMon_Handler This function handles Debug Monitor exception.*/
void DebugMon_Handler(void)
{
}

/*PendSV_Handler This function handles PendSVC exception.*/
void PendSV_Handler(void)
{
}

/*SysTick_Handler This function handles SysTick Handler.*/
void SysTick_Handler(void)
{
    HAL_IncTick();
    ms_counter++;
}

/*This function handles External line interrupt request for BlueNRG.*/
void BNRG_SPI_EXTI_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(BNRG_SPI_EXTI_PIN);
}

/*This function handles the Push Button interrupt request.*/
void PUSH_BUTTON_EXTI_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(KEY_BUTTON_PIN);
    button_event = 1;
}

/*This function handles RTC Auto wake-up interrupt request.*/
void RTC_WKUP_IRQHandler(void)
{
    HAL_RTCEx_WakeUpTimerIRQHandler(&RTCHandle);
}

/*This function handles RTC Alarm interrupt request.*/
void RTC_IRQHandler(void)
{
    HAL_RTC_AlarmIRQHandler(&RTCHandle);
}

void (*EXTI0_IRQFUNC)(void)=0;
#ifdef I2C_DMA_MODE
//..void (*EXTI9_5_IRQFUNC) (void)=imu_sensor_magneto_irq_callback;
#endif

#ifdef SENSOR_FIFO
/*lsm6ds3*/
void EXTI0_IRQHandler(void)
{
  if(__HAL_GPIO_EXTI_GET_IT(MEMS_INT1_PIN) != RESET)
  {
    __HAL_GPIO_EXTI_CLEAR_IT(MEMS_INT1_PIN);
    if(EXTI0_IRQFUNC)
      (*EXTI0_IRQFUNC)();
  }
}
#endif

#ifdef I2C_DMA_MODE
/*lsm303agr*/
void EXTI9_5_IRQHandler(void)
{
  if(__HAL_GPIO_EXTI_GET_IT(MAGNETO_INT1_PIN) != RESET)
  {
    __HAL_GPIO_EXTI_CLEAR_IT(MAGNETO_INT1_PIN);
   //if(EXTI9_5_IRQFUNC)
    //  (EXTI9_5_IRQFUNC) ();
  }
}

void DMA_STREAM_IRQHANDLER(void)
{
    HAL_DMA_IRQHandler(I2C_EXPBD_Handle.hdmarx);         
}
#endif

#ifdef INCLUDE_USB_DEVICE
extern PCD_HandleTypeDef hpcd;
extern USBD_HandleTypeDef USBD_Device;

void OTG_FS_IRQHandler(void)
{
  HAL_PCD_IRQHandler(&hpcd);
}

void OTG_FS_WKUP_IRQHandler(void)
{
  if((&hpcd)->Init.low_power_enable)
  {
    /* Reset SLEEPDEEP bit of Cortex System Control Register */
    SCB->SCR &= (uint32_t)~((uint32_t)(SCB_SCR_SLEEPDEEP_Msk | SCB_SCR_SLEEPONEXIT_Msk));  
    
    /* Configures system clock after wake-up from STOP: enable HSE, PLL and select 
    PLL as system clock source (HSE and PLL are disabled in STOP mode) */
    SystemClock_Config();
    
    /* ungate PHY clock */
     __HAL_PCD_UNGATE_PHYCLOCK((&hpcd)); 
  }
  /* Clear EXTI pending Bit*/
  __HAL_USB_OTG_FS_WAKEUP_EXTI_CLEAR_FLAG();
}
#endif

