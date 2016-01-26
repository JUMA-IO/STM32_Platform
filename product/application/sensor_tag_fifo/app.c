
#include "app.h"
#include "imu_sensor_fusion.h"
/*start adv*/

float pitch, roll, yal;

static void adv_name_generate(uint8_t* uni_name);
static void fifo_status(void* arg);
static void pitch_roll_yal_print(void* arg);
static void app_pitch_roll_yal_data_show(float* Data);
static void app_sensor_data_show(imu_sensor_data_t* Data);
static void app_uart_niming_report(uint8_t fun,uint8_t*data,uint8_t len);
static void app_pitch_roll(imu_sensor_data_t* data);

#ifdef CANNON_V2
char name[20] = "CANNON_V2";
#endif
#ifdef CANNON_V1
char name[20] = "CANNON_V1";
#endif

void on_ready(void)
{
    uint8_t tx_power_level = 7;
    uint16_t adv_interval = 100;

    uint8_t bdAddr[6];
    uint32_t data_rate = 800;
    HCI_get_bdAddr(bdAddr);
    adv_name_generate(bdAddr+4);
    /*Config Adv Parameter And Ready to Adv*/
    ble_set_adv_param(name, bdAddr, tx_power_level, adv_interval);
    ble_device_start_advertising();

    imu_sensor_select_features(ALL_ENABLE);

    imu_sensor_reset();

    imu_sensor_set_data_rate(&data_rate, LSM6DS3_XG_FIFO_MODE_FIFO);

    imu_sensor_start();

    run_after_delay(pitch_roll_yal_print, NULL, 1250);
}


static void adv_name_generate(uint8_t* uni_name) {
    char temp[3] = "_";
    /*adv name aplice*/
    sprintf(temp+1,"%01d%01d",*uni_name,*(uni_name+1));
    strcat(name, temp);
}

static void fifo_status(void* arg)
{
    imu_sensor_read_fifo_status();
//imu_sensor_read_data_from_fifo();
    run_after_delay(fifo_status, NULL, 1250);
}

static void app_pitch_roll(imu_sensor_data_t* data)
{

    complementary_filter(data->acc, data->gyro, data->mag, &pitch, &roll, &yal);
    //printf("pitch: %f, roll: %f, yal: %f \n", pitch, roll, yal);
}

static void app_pitch_roll_yal_data_show(float* Data)
{

}

static void app_sensor_data_show(imu_sensor_data_t* Data)
{

}

static void app_uart_niming_report(uint8_t fun,uint8_t*data,uint8_t len)
{
    uint8_t send_buf[32];
    uint8_t i;
    if(len>28)return;	//less than
    send_buf[len+3]=0;	//校验数置零
    send_buf[0]=0X88;	//帧头
    send_buf[1]=fun;	//功能字
    send_buf[2]=len;	//数据长度
    for(i=0; i<len; i++)send_buf[3+i]=data[i];			//复制数据
    for(i=0; i<len+3; i++)send_buf[len+3]+=send_buf[i];	//计算校验和
    //for(i=0;i<len+4;i++)usart1_send_char(send_buf[i]);	//发送数据到串口1
}

static void pitch_roll_yal_print(void* arg)
{
    printf("pitch: %f, roll: %f, yal: %f \n", pitch, roll, yal);
    run_after_delay(pitch_roll_yal_print, NULL, 1250);
}

/*received data callback*/
void on_imu_sensor_data(imu_sensor_data_t* data)
{
// printf("flag : %x,short :%x, sensor data: %x,%x,%x\n",
//           flag, data[0], data[1],data[2]);

    //printf("on imu_sensor_data\n");
    app_pitch_roll(data);

}

/* Device On Message */
void ble_device_on_message(uint8_t type, uint16_t length, uint8_t* value)
{


}
/* Device on connect */
void ble_device_on_connect(void)
{

   BLE_WAIT_REMOTE_ENABLE_NOTIFY;

}
/* Device on disconnect */
void ble_device_on_disconnect(uint8_t reason)
{
    /* Make the device connectable again. */
    Ble_conn_state = BLE_CONNECTABLE;
    ble_device_start_advertising();
}
