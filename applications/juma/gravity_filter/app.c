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
#include "imu_sensor_fusion.h"

static void adv_name_generate(uint8_t* uni_name);
void print_message(void* args);

char name[20] = "Cannon Gravity";
gravity_filter_context_t gravity_filter_context;

void on_ready(void)
{
    uint8_t tx_power_level = 5;
    uint16_t adv_interval = 100;
    uint8_t bdAddr[6];
    uint32_t data_rate = 800;

    HCI_get_bdAddr(bdAddr);
    adv_name_generate(bdAddr+4);
    /*Config Adv Parameter And Ready to Adv*/
    ble_set_adv_param(name, bdAddr, tx_power_level, adv_interval);
    ble_device_start_advertising();

    BSP_LED_On(LED0);
    imu_sensor_select_features(ALL_ENABLE);
    imu_sensor_reset();
    imu_sensor_set_data_rate(&data_rate, LSM6DS3_XG_FIFO_MODE_FIFO);
    gravity_filter_init(&gravity_filter_context);
    imu_sensor_start();
    run_after_delay(print_message, NULL, 0);
}

static void adv_name_generate(uint8_t* uni_name) {
    char temp[3] = "_";
    /*adv name aplice*/
    sprintf(temp+1,"%01d%01d",*uni_name,*(uni_name+1));
    strcat(name, temp);
}

/*received data callback*/
void on_imu_sensor_data(imu_sensor_data_t* data)
{
    gravity_filter_run(&gravity_filter_context, data);
}

void print_message(void* args)
{
    gravity_filter_context_t* cx = &gravity_filter_context;
    uint8_t tmp_buf[6];
    int32_t x,y,z;
    extern float test[3];

    run_after_delay(print_message, NULL, 100);

    x = (int32_t) cx->gravity.x;
    y = (int32_t) cx->gravity.y;
    z = (int32_t) cx->gravity.z;
    tmp_buf[0] = (x >> 8) & 0xFF;
    tmp_buf[1] = (x) & 0xFF;
    tmp_buf[2] = (y >> 8) & 0xFF;
    tmp_buf[3] = (y) & 0xFF;
    tmp_buf[4] = (z >> 8) & 0xFF;
    tmp_buf[5] = (z) & 0xFF;

    ble_device_send(0x01, 6, tmp_buf);
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
