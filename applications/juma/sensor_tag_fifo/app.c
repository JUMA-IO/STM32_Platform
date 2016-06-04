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
#include "juma_sensor.h"
#include "imu_sensor.h"
#include "imu_sensor_fusion.h"

sensor_fusion_angle_t sensor_angle[1] = {0};
static imu_sensor_fusion_1_context_t imu_sensor_context = {
        0.02,
        0.001,
        0.98,
        0.02,
        0.02,
        0.02,
        0.0001,
        0.0,
        0.0,
        0.0,
};

static void adv_name_generate(uint8_t* uni_name);
static void pitch_roll_yal_print(void* arg);
static void app_pitch_roll(imu_sensor_data_t* data);

char name[20] = "Cannon STagFifo";

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
    imu_sensor_start();
    run_after_delay(pitch_roll_yal_print, NULL, 110);
}

static void adv_name_generate(uint8_t* uni_name) {
    char temp[3] = "_";
    /*adv name aplice*/
    sprintf(temp+1,"%01d%01d",*uni_name,*(uni_name+1));
    strcat(name, temp);
}

static void app_pitch_roll(imu_sensor_data_t* data)
{    
    imu_sensor_fusion_1(data, sensor_angle, &imu_sensor_context);
}

static void pitch_roll_yal_print(void* arg)
{
    uint8_t tmp_buf[6];
    tmp_buf[0] = (((int32_t)sensor_angle->pitch) >> 8) & 0xFF;
    tmp_buf[1] = ((int32_t)sensor_angle->pitch) & 0xFF;
    tmp_buf[2] = (((int32_t)sensor_angle->roll) >> 8) & 0xFF;
    tmp_buf[3] = ((int32_t)sensor_angle->roll) & 0xFF;
    tmp_buf[4] = (((int32_t)sensor_angle->yaw) >> 8) & 0xFF;
    tmp_buf[5] = ((int32_t)sensor_angle->yaw) & 0xFF;
    ble_device_send(0x01, 6, tmp_buf);
    run_after_delay(pitch_roll_yal_print, NULL, 110);
}

/*received data callback*/
void on_imu_sensor_data(imu_sensor_data_t* data)
{
    app_pitch_roll(data);
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
