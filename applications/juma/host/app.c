//#include "sensor_service.h"

#include "app.h"

#if NO_PRINTF
#define printf(...)
#endif

#ifdef CANNON_V2
char name[20] = "CANNON_V2";
#endif
#ifdef CANNON_V1
char name[20] = "CANNON_V1";
#endif

void on_ready(void)
{
    uint8_t tx_power_level = 7;
    uint16_t scan_interval = 100;

    uint8_t bdAddr[6] = {0x03,0x03,0x03,0x03,0x03,0x03};
    /*Host*/
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
