//#include "sensor_service.h"

#include "app.h"
/*start adv*/

extern volatile uint8_t device_connectable;
static void sensor_read(void* arg);
static void adv_name_generate(uint8_t* uni_name);

#ifdef CANNON_V2
char name[] = "CANNON_V2";
#endif
#ifdef CANNON_V1
char name[] = "CANNON_V1";
#endif
uint8_t tx_power_level = 7;
uint16_t adv_interval = 100;
uint16_t scan_interval = 100;

void on_ready(void)
{
    uint8_t bdAddr[6] = {0x03,0x03,0x03,0x03,0x03,0x03};
#ifdef CLIENT_ROLE
    /*Host*/
    ble_host_set_scan_param(bdAddr, tx_power_level, scan_interval);
    ble_host_start_scan();
#else
    HCI_get_bdAddr(bdAddr);
    adv_name_generate(bdAddr+4);
    /*Config Adv Parameter And Ready to Adv*/
    ble_set_adv_param(name, bdAddr, tx_power_level, adv_interval);
    ble_device_start_advertising();
#endif
    //run_after_delay(sensor_read, NULL, 500);
}

static void adv_name_generate(uint8_t* uni_name){
  char temp[3] = "_";
   /*adv name aplice*/
  sprintf(temp+1,"%d%d",*uni_name,*(uni_name+1));
  strcat(name, temp);
}

void ble_host_found_device_info(scan_device_found_info device_info)
{
    if(device_info.RSSI > -60) {
        device_connectable = CONNECT_DEVICE_ENABLE;
    } else {
        ble_host_start_scan();
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

/* Device On Message */
void ble_device_on_message(uint8_t type, uint16_t length, uint8_t* value)
{

}
/* Device on connect */
void ble_device_on_connect(void)
{

    tBleStatus ret = BLE_WAIT_REMOTE_ENABLE_NOTIFY;

}
/* Device on disconnect */
void ble_device_on_disconnect(uint8_t reason)
{
    /* Make the device connectable again. */
    Ble_conn_state = BLE_CONNECTABLE;
    ble_device_start_advertising();
}
