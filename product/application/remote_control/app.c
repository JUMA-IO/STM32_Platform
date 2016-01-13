
#include "app.h"
/*start adv*/

const char *name = "Remote Control";
uint8_t adv_address[] = {0x08, 0x05, 0x04, 0x03, 0x02, 0x04};
uint8_t tx_power_level = 7;
uint16_t adv_interval = 100;
static float humidity;
static float temperature;

void on_ready(void)
{
    /*Config Adv Parameter And Ready to Adv*/
    ble_set_adv_param(name, adv_address, tx_power_level, adv_interval);
    ble_device_start_advertising();

}

void user_process(void)
{

}

/* Device On Message */
void ble_device_on_message(uint8_t type, uint16_t length, uint8_t* value)
{
    /*LED Control*/
    if(*value == 0x00) {
        BSP_LED_On(LED0);
    } else if(*value == 0x01) {
        BSP_LED_Off(LED0);
    }

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
}
