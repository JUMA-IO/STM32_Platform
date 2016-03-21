
#include "app.h"
/*start adv*/

char *name = "CAF_ECHO";
uint8_t adv_address[6] = {0x08, 0x05, 0x04, 0x03, 0x02, 0x04};

void on_ready(void)
{
    uint8_t tx_power_level = 7;
    uint16_t adv_interval = 100;

    /*Config Adv Parameter And Ready to Adv*/
    ble_set_adv_param(name, adv_address, tx_power_level, adv_interval);
    ble_device_start_advertising();

}

/* Device On Message */
void ble_device_on_message(uint8_t type, uint16_t length, uint8_t* value)
{

    /*echo data*/
    ble_device_send(type, length, value);

}
/* Device on connect */
void ble_device_on_connect(void)
{


}
/* Device on disconnect */
void ble_device_on_disconnect(uint8_t reason)
{
    /* Make the device connectable again. */
    Ble_conn_state = BLE_CONNECTABLE;
    ble_device_start_advertising();
}
