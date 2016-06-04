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

/*start adv*/
char *name = "Template";
uint8_t adv_address[] = {0x08, 0x05, 0x04, 0x03, 0x02, 0x04};	
uint8_t tx_power_level = 7;
uint16_t adv_interval = 100;
void on_ready(void)
{
	/*Config Adv Parameter And Ready to Adv*/
	ble_set_adv_param(name, adv_address, tx_power_level, adv_interval);
	ble_device_start_advertising();
}
/* Device On Message */
void ble_device_on_message(uint8_t type, uint16_t length, uint8_t* value)
{		
}
/* Device on connect */
void ble_device_on_connect(void)
{
}
/* Device on disconnect */
void ble_device_on_disconnect(uint8_t reason)
{
	/* Make the device connectable again. */
	ble_device_start_advertising();
}
