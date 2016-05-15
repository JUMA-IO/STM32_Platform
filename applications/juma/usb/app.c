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

static void app_led_control(uint8_t flag);

#include "usbd_core.h"
#include "usbd_hid.h"
#include "usbd_desc.h"
#include "usbd_conf.h"

USBD_HandleTypeDef USBD_Device;
uint8_t HID_Buffer[4];

void on_ready(void)
{
  /* Init Device Library */
  USBD_Init(&USBD_Device, &HID_Desc, 0);
  
  /* Add Supported Class */
  USBD_RegisterClass(&USBD_Device, USBD_HID_CLASS);
  
  /* Start Device Process */
  USBD_Start(&USBD_Device);

	//run_after_delay(sensor_read, NULL, 500);
}

static void app_led_control(uint8_t flag)
{
    if(flag == 0x00) {
        BSP_LED_On(LED0);
    }
    if(flag == 0x01) {
        BSP_LED_Off(LED0);
    }
}