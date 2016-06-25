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
#ifndef _BLUENRG_SDK_API_H_
#define _BLUENRG_SDK_API_H_

/*scan interval*/
#define SCAN_P (0x4000)
#define SCAN_L (0x4000)
/*supervision timeout, in sec.*/
#define SUPERV_TIMEOUT (60)
/*connection period, arg in msec.*/
#define CONN_P(x) ((int)((x)/1.25f))
/*connection length, arg in msec.*/
#define CONN_L(x) ((int)((x)/0.625f))
#define CONN_P1     (CONN_P(50))
#define CONN_P2     (CONN_P(50))
#define CONN_L1     (CONN_L(1250))
#define CONN_L2     (CONN_L(1250))
/*connection handle*/
#define MASTER_ROLE  0x00
#define SLAVE_ROLE    0x01
#define VARIABLE_SIZE 20

typedef	uint8_t	tBDAddr[6];
typedef struct scan_device_found {
    uint8_t	bdaddr_type;  /**< Type of the peer address (@ref PUBLIC_ADDR, @ref RANDOM_ADDR). */
    tBDAddr	bdaddr;
    uint8_t	local_name_len;
    uint8_t local_name[VARIABLE_SIZE];
    uint8_t	uuid_type; /**< Advertising or scan response data + RSSI. RSSI is last octect (signed integer). */
    uint8_t uuid[VARIABLE_SIZE];
    int8_t RSSI;
} scan_device_found_info;

typedef struct _disconn_event_pckt{
  uint8_t status;
  uint16_t conn_handle;
  uint8_t  disconn_reason;
} disconn_event_pckt;

typedef enum {
    SCAN_FILTER_DUPLICATE_DISABLE = 0, /**< scan filter duplicate disable. */
    SCAN_FILTER_DUPLICATE_ENABLE = 1  /**< scan filter duplicate enable. */
} SCAN_FilterDuplicateType;

typedef enum {
    SCAN_PASSIVE = 0, /* SERVER is for Peripheral role. */
    SCAN_ACTIVE = 1   /* CLIENT is for Central role. */
} SCAN_Type;

typedef enum {
    CLIENT = 0x01, /* CLIENT is for Central role. */
    SERVER = 0x02, /* SERVER is for Peripheral role. */
    CLIENT_SERVER = 0x03
} BLE_RoleTypeDef;


typedef void (*function_t)(void* args);
uint32_t current_time(void);
void run_when_idle(function_t func, void* args);
void run_at_time(function_t func, void* args, uint32_t time);
#define run_after_delay(func, args, delay) \
	run_at_time((func), (args), current_time() + (delay))

void GAP_DisconnectionComplete_CB(void);
void GAP_ConnectionComplete_CB(uint8_t addr[6], uint16_t handle);
void GATT_Notification_CB(uint16_t attr_handle, uint8_t attr_len, uint8_t *attr_value);


void on_ready(void);
uint8_t ble_init_bluenrg(void);
uint8_t ble_set_tx_power(uint8_t level);

/*set adv namee*/
uint8_t ble_device_set_name(const char* new_device_name);
/*config adv interval (min_adv_interval > 32*0.625)*/
void ble_device_set_advertising_interval(uint16_t interval);
/*add adv address*/
uint8_t ble_address(uint8_t* bdaddr);
/*start to adv*/
uint8_t ble_device_start_advertising(void);
/*set adv param*/
void ble_set_adv_param(char* adv_name, uint8_t* bdaddr, uint8_t tx_power_level, uint16_t adv_interval);
/*stop adv*/
uint8_t ble_device_stop_advertising(void);
/*to disconnect ble connection*/
uint8_t ble_disconnect_device(void);
/*tx data(notify)*/
uint8_t ble_device_send(uint8_t type, uint32_t length, uint8_t* value);
/*rx data(write / write without responce)*/
void ble_device_on_message(uint8_t type, uint16_t length, uint8_t* value);
void ble_device_on_connect( void );
void ble_device_on_disconnect(uint8_t reason);


#endif //_BLUENRG_SDK_API_H_
