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
#ifndef _MESH_H_
#define _MESH_H_

typedef struct _mesh_manuf_data_t{
 
    uint8_t   len;              //total data length
    uint8_t   data_type;        // advertise data type
    uint16_t  adv_info_number;  // advertising information number
    uint16_t  data;             // user data
}mesh_manuf_data_t;

enum mesh_status_t {
	mesh_ok = 0,
	mesh_failed = 1,
};

/*mesh rx scan data handle*/
void mesh_rx_scan_data(void* data);
/*mesh rx host message handle*/
void mesh_rx_host_message(void* data);
/*mesh disconnect handle*/
void mesh_disconnect_handle(void* data);
/*mesh on message*/
void mesh_on_message(void* data);
#endif





