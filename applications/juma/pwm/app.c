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
#include "app.h"
#include "bsp_common.h"
#include "bluenrg_sdk_api.h"
#include "pwm.h"

TIM_HandleTypeDef TimHandleT2;
int16_t LED_luminance=0;
 
void breathing_led_init()
{
	TIM_OC_InitTypeDef pwmConfig;
  GPIO_InitTypeDef GPIO_InitStruct;

  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_TIM2_CLK_ENABLE();
  GPIO_InitStruct.Pin = GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
  TimHandleT2.Instance = TIM2;
  TimHandleT2.Init.Period =  1000 - 1;
  TimHandleT2.Init.Prescaler = 168-1;
  TimHandleT2.Init.ClockDivision = 0;
  TimHandleT2.Init.CounterMode = TIM_COUNTERMODE_UP;  
  HAL_TIM_PWM_Init(&TimHandleT2);

  pwmConfig.OCMode=TIM_OCMODE_PWM1;
  pwmConfig.Pulse=0;
  HAL_TIM_PWM_ConfigChannel(&TimHandleT2, &pwmConfig, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&TimHandleT2, TIM_CHANNEL_2);
}

void breathing_led_run()
{
	LED_luminance+=10;
	if(LED_luminance>=1000)LED_luminance=-1000;
	TIM_PWM_Duty(&TimHandleT2,TIM_CHANNEL_2,abs(LED_luminance));
	run_after_delay(breathing_led_run, NULL, 10);
}

void on_ready(void)
{
	breathing_led_init();
	breathing_led_run();
}


