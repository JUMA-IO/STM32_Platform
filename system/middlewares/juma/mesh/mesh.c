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
#include "mesh.h"
#include "bluenrg_sdk_api.h"
#include "bluenrg_gap.h"

#if NO_PRINTF
#define printf(...)
#endif

uint16_t adv_info_number = 1;

/*manufacturing data in advertise payload*/
static mesh_manuf_data_t  mesh_manuf_data = {
    5,                                  //length
    AD_TYPE_MANUFACTURER_SPECIFIC_DATA,  //adv data type
    0x0001,                              //advertise information number
    0x000F                               // data
};

static void mesh_disconn_connection(void* data)
{
    ble_disconnect_device();
}

static void mesh_manuf_data_config(void* data)
{
     mesh_manuf_data_t* rx_data = data;
     
     adv_info_number++;
     mesh_manuf_data.adv_info_number = adv_info_number;
     mesh_manuf_data.data = rx_data->data;
     printf("mesh_manuf_data_config:data:%x\n", mesh_manuf_data.data);
}

/*set manufacturing data on the advertising packet*/
static void mesh_adv_data_update(void* arg)
{
    tBleStatus ret;   
    ret = aci_gap_update_adv_data(6, (uint8_t*)&mesh_manuf_data);
    if(ret){
        printf("aci_gap_update_adv_data failed \n");
    }
}

void mesh_rx_scan_data(void* data)
{
      mesh_manuf_data_t* rx_data = data;
  
    if(rx_data->adv_info_number > adv_info_number){
      
       run_when_idle(mesh_on_message, &(rx_data->data));
       run_when_idle(mesh_manuf_data_config, data);
       run_when_idle(mesh_adv_data_update, NULL);
    }
}

void mesh_rx_host_message(void* data)
{
    mesh_manuf_data_t* rx_data = data;
    /*disconnect master connection*/
    run_after_delay(mesh_disconn_connection, NULL, 10);  
    run_when_idle(mesh_on_message, &(rx_data->data));
    run_when_idle(mesh_manuf_data_config, data);
}

void mesh_disconnect_handle(void* data)
{
    tBleStatus ret;
 
    ret = ble_host_stop_scan();
    if(ret){
       printf("ble_host_stop_scan failed \n");
    }
    
    ret = ble_device_start_advertising();
    if(ret){
       printf("ble_device_start_advertising \n");
    }
    
    run_when_idle(mesh_adv_data_update, NULL);
}
