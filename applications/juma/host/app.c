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

#if NO_PRINTF
#define printf(...)
#endif

char name[20] = "Cannon Host";

void on_ready(void)
{
    uint8_t tx_power_level = 7;
    uint16_t scan_interval = 100;

    uint8_t bdAddr[6] = {0x03,0x03,0x03,0x03,0x03,0x03};
    ble_host_set_scan_param( tx_power_level, bdAddr, scan_interval);
    ble_host_start_scan(NULL);
}

void ble_host_on_device_info(void* arg)
{
    scan_device_found_info* device_info = arg;
    if(device_info->RSSI > -60) {
        ble_host_connect(device_info->bdaddr);
    } else {
        ble_host_start_scan(NULL);
    }
}

void ble_host_on_message(uint8_t type, uint16_t length, uint8_t* value)
{
    BSP_LED_Toggle(LED0);
    /*echo*/
    *value = 1;
    ble_host_send(type, length, value);
    printf("receive data:%x,%x,%x\n\r",type,length,value[0]);
}

void ble_host_on_connect()
{
    uint8_t data = 1;
    ble_host_send(0x01, 0x01, &data);
    printf("send\n\r");

}
