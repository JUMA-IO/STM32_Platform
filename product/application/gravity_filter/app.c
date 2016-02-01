
#include "app.h"
#include "imu_sensor_fusion.h"
/*start adv*/

#if 0 //NO_PRINTF
#define printf(...)
#endif

static void adv_name_generate(uint8_t* uni_name);
void print_message(void* args);

#ifdef CANNON_V2
char name[20] = "CANNON_V2";
#endif
#ifdef CANNON_V1
char name[20] = "CANNON_V1";
#endif

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

    run_when_idle(print_message, NULL);
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

    printf("%f %f %f\r\n", cx->gravity.x, cx->gravity.y, cx->gravity.z);
    
    run_after_delay(print_message, NULL, 500);
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
    Ble_conn_state = BLE_CONNECTABLE;
    ble_device_start_advertising();
}
