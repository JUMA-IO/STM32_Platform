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
#include "pwm.h"

/**
  * @brief  Init TIM PWM .
  * @param  htim: pointer to a TIM_HandleTypeDef structure that contains
  *                the configuration information for TIM module.
  * @param  Channel: TIM Channels to be enabled.
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_3: TIM Channel 3 selected
  *            @arg TIM_CHANNEL_4: TIM Channel 4 selected
	* @param  GPIOx:   x can be (A..D) to select the GPIO peripheral .                    
  * @param  GPIO_Pin: specifies the port bit to be written.
  *          This parameter can be one of GPIO_PIN_x where x can be (0..15).
	* @param  Freq:	specifies the PWM frequency to be written.
	* @param  Pulse: specifies the PWM duty cycle to be written.
  * @retval HAL status
  */
HAL_StatusTypeDef TIM_PWM_Init(TIM_HandleTypeDef *htim,  uint32_t Channel,GPIO_TypeDef* GPIOx, 
																uint16_t GPIO_Pin,uint32_t Freq,uint32_t Pulse)
{
	return HAL_OK;
}

/**
  * @brief  set PWM duty cycle.
  * @param  htim: pointer to a TIM_HandleTypeDef structure that contains
  *                the configuration information for TIM module.
  * @param  Channel: TIM Channels to be enabled.
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_3: TIM Channel 3 selected
  *            @arg TIM_CHANNEL_4: TIM Channel 4 selected
  * @retval HAL status
  */
HAL_StatusTypeDef TIM_PWM_Duty(TIM_HandleTypeDef *htim,  uint32_t Channel,uint32_t Pulse){
	
	 /* Check the parameters */
  assert_param(IS_TIM_CCX_INSTANCE(htim->Instance, Channel));
	
  switch (Channel)
  {
    case TIM_CHANNEL_1:
    {
			/* Set the Capture Compare Register value */
			htim->Instance->CCR1 =Pulse;
    }break;
    case TIM_CHANNEL_2:
    {
			 /* Set the Capture Compare Register value */
       htim->Instance->CCR2 =Pulse;
    }break;
    case TIM_CHANNEL_3:
    {
			 /* Set the Capture Compare Register value */
       htim->Instance->CCR3 =Pulse;
    }break;
    case TIM_CHANNEL_4:
    {
			 /* Set the Capture Compare Register value */
       htim->Instance->CCR4 =Pulse;
    }break;
    default:
    break;  
  }
  return HAL_OK;
}
