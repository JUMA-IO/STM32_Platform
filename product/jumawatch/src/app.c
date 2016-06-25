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
#include "x_nucleo_iks01a1_hum_temp.h"
#include "x_nucleo_iks01a1_pressure.h"
#include "x_nucleo_iks01a1_imu_6axes.h"
#include "hts221.h"
#include "lps25hb.h"
#include "lps25h.h"
#include "lsm6ds3.h"
#include "lsm303agr.h"
#include "juma_sensor.h"
#include "st7789v.h"

#define UPDATE_INTERVAL 100

static void app_sensor_check(void);
static void read_pressure(void* arg);
static void read_temperature(void* arg);
static void read_humidity(void* arg);
static void read_mag(void* arg);
static void read_acc(void* arg);
static void read_gyro(void* arg);
static void lcd_set_cursor(uint16_t x, uint16_t y);
static void lcd_fill_color(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey);
static void lcd_set_vram(uint8_t area, uint8_t mode);
/*start adv*/

char *name = "WATCH";
uint8_t bdaddr[6] = {0x01, 0x01, 0x01, 0x01, 0x01, 0x01};
static uint8_t running;
static uint16_t temp_data[100] = {0x6455, 0x6455, 0x6456, 0x6476, 0x6476, 0x6476, 0x6476, 0x6476, 0x6476, 0x6476, 0x6C76, 0x6C96, 0x6C96, 0x6C96, 0x6C96, 0x6C96, 0x6C96, 0x6C96, 0x6C96, 0x6C96, 0x6CB6, 0x6CB6, 0x6CB6, 0x6CB6, 0x6CB6, 0x6CB6, 0x6CB6, 0x74B7, 0x74B7, 0x74B7, 
        0x74D7, 0x74D7, 0x74D7, 0x74D7, 0x74D7, 0x74D7, 0x74D7, 0x74D7, 0x74D7, 0x74F7, 0x74F7, 0x74F7, 0x74F7, 0x74F7, 0x7CF7, 0x7CF7, 0x7CF7, 0x7CF7, 0x7CF7, 0x7D17, 0x7D17, 0x7D17, 0x7D17, 0x7D17, 0x7D17, 0x7D17, 0x7D17, 0x7D17, 0x7D17, 0x7D17, 
        0x7D17, 0x7D17, 0x7D17, 0x7D17, 0x7D17, 0x7D17, 0x7D17, 0x7D17, 0x7CF7, 0x7CF7, 0x7CF7, 0x7CF7, 0x7CF7, 0x74F7, 0x74F7, 0x74F7, 0x74F7, 0x74F7, 0x74F7, 0x74F7, 0x74F7, 0x74D7, 0x74D7, 0x74D7, 0x74D7, 0x74D7, 0x74D7, 0x74D7};

uint32_t data;
void on_ready(void)
{
    uint8_t tx_power_level = 7;
    uint16_t adv_interval = 100;

    /*Config Adv Parameter And Ready to Adv*/
    ble_set_adv_param(name, bdaddr, tx_power_level, adv_interval);
    ble_device_start_advertising();
    /*check sensor*/
    app_sensor_check();
		while(1){
			lcd_fill_color(0,0,200,200);
			data = LCD_IO_ReadData(0x04,4);
			HAL_Delay(10);
		}
		lcd_set_vram(100,3);
}

static void lcd_set_vram(uint8_t area, uint8_t mode)
{
		st7789v_WriteReg(0x33);
		st7789v_WriteData(0x00);
		st7789v_WriteData(0x00);
		st7789v_WriteData(0xC8);
		st7789v_WriteData(0xC8);
		st7789v_WriteData(0x00);
		st7789v_WriteData(0x00);
		st7789v_WriteReg(0x37);
		st7789v_WriteData(0x64);
		st7789v_WriteData(0x03);
}
static void lcd_setRow_cursor(uint16_t xpos, uint16_t ypos)
{
		st7789v_WriteReg(0x2A); 
		st7789v_WriteData(xpos>>8);st7789v_WriteData(xpos&0XFF); 			 
		st7789v_WriteData(ypos>>8);st7789v_WriteData(ypos&0XFF); 		
}
static void lcd_setCol_cursor(uint16_t xpos, uint16_t ypos)
{
		st7789v_WriteReg(0x2A); 
		st7789v_WriteData(xpos>>8);st7789v_WriteData(xpos&0XFF); 			  
		st7789v_WriteData(ypos>>8);st7789v_WriteData(ypos&0XFF); 		
}
static void lcd_fill_color(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey)
{
     uint16_t i,j;
     uint16_t xlen=0;
     uint16_t temp;
 
		lcd_setRow_cursor(sx,ex);
		lcd_setRow_cursor(sy,ey);
		st7789v_WriteReg(0x2C);
		for(i=0;i<=90;i++)
		{
		 st7789v_WriteData(temp_data[i]);
		}
			//		xlen=ex-sx+1;	
//		st7789v_WriteReg(0x3A);
//    st7789v_WriteData(0x05);
//		for(i=sy;i<=ey;i++)
//		{
//      lcd_set_cursor(sx,i);
//    
//      for(j=0;j<xlen;j++){ 
//       lcd_set_cursor(sy,j);
//			 st7789v_WriteReg(0x2C);
//			 st7789v_WriteData(0x01);
//     };    
	
//	}	 
     
}

/*init hts221*/
static void hum_temp_monitor_init(void)
{
    BSP_HUM_TEMP_Init();
}

/*init lps25hb*/
static void pressure_sensor_init(void)
{
    BSP_PRESSURE_Init();
}

/*init lsm6ds3*/
static void lsm6ds3_6_axis_init(void)
{
    BSP_IMU_6AXES_Init();
}

/*init lsm303agr*/
static void lsm303agr_init(void)
{
    int8_t ret;
    ret = init_LSM303AGR_mag(LSM303AGR_MAG_ODR_100Hz);
    if(ret !=0x00){
        return;
    }
}

static void app_sensor_check(void)
{
    uint8_t ret,tmp1 = 0x55;
    /*init*/
		//hum_temp_monitor_init();
    pressure_sensor_init();
    lsm6ds3_6_axis_init();
    lsm303agr_init();
    /*check hts221*/
    ret = BSP_HUM_TEMP_CheckID();
    if(ret != APP_OK)
    {
        while(1);
    }
    /*check lps25hb*/
    ret =BSP_PRESSURE_CheckID();
    if(ret != APP_OK)
    {
        while(1);
    }   
    /*check ls6ds3*/
    ret = BSP_IMU_6AXES_Check_XG_ID();
    if(ret != APP_OK)
    {
        while(1);
    } 
    /*check lsm303agr*/
    ret = LSM303AGR_MAG_R_WHO_AM_I(&tmp1);
    if(ret != APP_OK)
    {
        while(1);
    }
    if(tmp1 != LSM303AGR_MAG_WHO_AM_I)
    {
        while(1);
    }
}

static void read_temperature(void* arg)
{
    int16_t humidity;
    int16_t temperature;
    JSensor_HUM_TEMP_Typedef tdef;

    if (!running) return;
    tdef.humidity = &humidity;
    tdef.temperature = &temperature;
    if (JSENSOR_OK == jsensor_app_read_sensor(JSENSOR_TYPE_HUMITY_TEMP, (void *)&tdef)) {
        ble_device_send(0x00, 2, (uint8_t *)&temperature);
    }
    run_after_delay(read_humidity, NULL, UPDATE_INTERVAL);
}

static void read_humidity(void* arg)
{
    int16_t humidity;
    int16_t temperature;
    JSensor_HUM_TEMP_Typedef tdef;

    if (!running) return;
    tdef.humidity = &humidity;
    tdef.temperature = &temperature;
    if (JSENSOR_OK == jsensor_app_read_sensor(JSENSOR_TYPE_HUMITY_TEMP, (void *)&tdef)) {
        ble_device_send(0x01, 2, (uint8_t *)&humidity);
    }

    read_pressure(NULL);
}

static void read_pressure(void* arg)
{
    JSensor_Press_Typedef tdef;
    int32_t pressure;

    if (!running) return;
    tdef.pressure = &pressure;
    if (JSENSOR_OK == jsensor_app_read_sensor(JSENSOR_TYPE_PRESSURE, (void *)&tdef)) {
        ble_device_send(0x02, 3, (uint8_t *)&pressure);
    }

    run_after_delay(read_mag, NULL, UPDATE_INTERVAL);
}

static void read_mag(void* arg)
{
    JSensor_MAG_Typedef tdef;
    int8_t  MAG[6];

    if (!running) return;
    tdef.MAG = MAG;
    if(JSENSOR_OK == jsensor_app_read_sensor(JSENSOR_TYPE_MAGNET, (void *)&tdef)) {
        ble_device_send(0x03, 6, (uint8_t*)MAG);
        //printf("%x,%x,%x,%x,%x,%x\n\r", MAG[0],MAG[1],MAG[2],MAG[3],MAG[4],MAG[5]);
    }

    run_after_delay(read_acc, NULL, UPDATE_INTERVAL);
}

static void read_acc(void* arg)
{
    JSensor_AXIS_Typedef tdef;
    int8_t ACC[6], GRO[6];

    if (!running) return;
    tdef.ACC = ACC;
    tdef.GRO = GRO;
    if (JSENSOR_OK == jsensor_app_read_sensor(JSENSOR_TYPE_MOTION_6AXIS, (void *)&tdef)) {
        ble_device_send(0x04, 6, (uint8_t*)ACC);
    }

    run_after_delay(read_gyro, NULL, UPDATE_INTERVAL);
}

static void read_gyro(void* arg)
{
    JSensor_AXIS_Typedef tdef;
    int8_t ACC[6], GRO[6];

    if (!running) return;
    tdef.ACC = ACC;
    tdef.GRO = GRO;
    if (JSENSOR_OK == jsensor_app_read_sensor(JSENSOR_TYPE_MOTION_6AXIS, (void *)&tdef)) {
        ble_device_send(0x05, 6, (uint8_t*)GRO);
    }

    run_after_delay(read_temperature, NULL, UPDATE_INTERVAL);
}

void ble_device_on_message(uint8_t type, uint16_t length, uint8_t* value)
{
    /*echo data*/
    ble_device_send(type, length, value);
}

void ble_device_on_connect(void)
{
    running = 1;
    run_after_delay(read_temperature, NULL, UPDATE_INTERVAL);
}

void ble_device_on_disconnect(uint8_t reason)
{
    /* Make the device connectable again. */
    ble_device_start_advertising();
}
