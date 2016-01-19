
#include "app.h"
#include "imu_sensor_fusion.h"
/*start adv*/

float pitch, roll, yal;

static void sensor_read(void* arg);
static void adv_name_generate(uint8_t* uni_name);
static void fifo_status(void* arg);
static void pitch_roll_yal_print(void* arg);

#ifdef CANNON_V2
char name[] = "CANNON_V2";
#endif
#ifdef CANNON_V1
char name[] = "CANNON_V1";
#endif
uint8_t tx_power_level = 7;
uint16_t adv_interval = 100;


void on_ready(void)
{
    uint8_t bdAddr[6];
     uint32_t data_rate = 800;
    HCI_get_bdAddr(bdAddr);
    adv_name_generate(bdAddr+4);
    /*Config Adv Parameter And Ready to Adv*/
    ble_set_adv_param(name, bdAddr, tx_power_level, adv_interval);
    ble_device_start_advertising();
    
    imu_sensor_select_features(ACC_AND_GYRO_ENABLE);   
    
    imu_sensor_reset();
    
    imu_sensor_set_data_rate(&data_rate);
    
    imu_sensor_start();
    
    run_after_delay(pitch_roll_yal_print, NULL, 500);
}


static void adv_name_generate(uint8_t* uni_name) {
    char temp[3] = "_";
    /*adv name aplice*/
    sprintf(temp+1,"%d%d",*uni_name,*(uni_name+1));
    strcat(name, temp);
}

static void fifo_status(void* arg)
{
 imu_sensor_read_fifo_status();
 //imu_sensor_read_data_from_fifo();
 run_after_delay(fifo_status, NULL, 100);
}

void app_pitch_roll(imu_sensor_data_t* Data)
{
   
   complementary_filter(Data->acc, Data->gyro, Data->mag, &pitch, &roll, &yal);
   //printf("pitch: %f, roll: %f, yal: %f \n", pitch, roll, yal);
}

static void pitch_roll_yal_print(void* arg)
{
   printf("pitch: %f, roll: %f, yal: %f \n", pitch, roll, yal);
   run_after_delay(pitch_roll_yal_print, NULL, 500);
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
