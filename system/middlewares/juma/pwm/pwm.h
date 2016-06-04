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
#ifndef _PWM_H_
#define _PWM_H_

HAL_StatusTypeDef TIM_PWM_Init(TIM_HandleTypeDef *htim,  uint32_t Channel,GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin,uint32_t Freq,uint32_t Pulse);
HAL_StatusTypeDef TIM_PWM_Duty(TIM_HandleTypeDef *htim,  uint32_t Channel,uint32_t Pulse);

#endif //_PWM_H_
