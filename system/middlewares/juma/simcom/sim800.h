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

#ifndef __SIM800_H
#define __SIM800_H

#include "bsp_common.h" 
#include "stm32f4xx_hal_uart.h"
#include "stm32f4xx_hal_msp.h" 


int sim800_init();
int sim800_deinit();
int sim800_test_phone_call(const char * phone);
int sim800_http_post_chicken(const char * did, uint32_t steps, uint32_t volt);

#endif /* __SIM800_H */

 
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

