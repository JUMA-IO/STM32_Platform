
#include "app.h"
#include "imu_sensor_fusion_9-axis.h"
#include "stm32f4xx_hal_msp.h"

#include <math.h>

#if NO_PRINTF
#define printf(...)
#endif

#define SAMPLINGRATE 400

static void adv_name_generate(uint8_t* uni_name);
static void sensor_print(void* arg);

static uint8_t sendFLAG = FALSE;
static uint8_t sensor_data_ready_FLAG = FALSE;
static imu_sensor_data_t local_data;

static float q[4] = {1.0f, 0.0f, 0.0f, 0.0f};

#ifdef CANNON_V2
char name[20] = "CANNON_V2";
#endif
#ifdef CANNON_V1
char name[20] = "CANNON_V1";
#endif

void on_ready(void)
{
  uint8_t tx_power_level = 5;
  uint16_t adv_interval = 100;

  uint8_t bdAddr[6];
  uint32_t data_rate = SAMPLINGRATE;
  HCI_get_bdAddr(bdAddr);
  adv_name_generate(bdAddr + 4);
  /*Config Adv Parameter And Ready to Adv*/
  ble_set_adv_param(name, bdAddr, tx_power_level, adv_interval);
  ble_device_start_advertising();

  BSP_LED_Off(LED0);

  imu_sensor_9_axis_reset();

  imu_sensor_9_axis_set_data_rate(&data_rate, LSM6DS3_XG_FIFO_MODE_CONTINUOUS_OVERWRITE);

  imu_sensor_9_axis_start();

}


static void adv_name_generate(uint8_t* uni_name) {
  char temp[3] = "_";
  /*adv name aplice*/
  sprintf(temp + 1, "%01d%01d", *uni_name, *(uni_name + 1));
  strcat(name, temp);
}


static void sensor_print(void* arg)
{
  static int counter = 0;
  if (++counter > 5) {
    counter = 0;

    //static char st = '$';
    //UART_Transmit((uint8_t *)&st, 1);
    //UART_Transmit((uint8_t *)q, sizeof(q));
    if (sendFLAG) {
      ble_device_send(0x01, 16, (uint8_t*)q);
    }
  }
}

/*received data callback*/
void on_imu_sensor_9_axis_data(imu_sensor_data_t* data)
{
  local_data = *data;
  sensor_data_ready_FLAG = TRUE;

  //quick jump out sensor ISR for other events
}

/* Device On Message */
void ble_device_on_message(uint8_t type, uint16_t length, uint8_t* value)
{


}
/* Device on connect */
void ble_device_on_connect(void)
{
  sendFLAG = TRUE;

}
/* Device on disconnect */
void ble_device_on_disconnect(uint8_t reason)
{
  sendFLAG = FALSE;
  /* Make the device connectable again. */
  Ble_conn_state = BLE_CONNECTABLE;
  ble_device_start_advertising();
}


void loop(){
	static int i=0;
	static float goffsetx=0,goffsety=0,goffsetz=0;
	static float coef = 3.1415926599f/180;
	if(sensor_data_ready_FLAG){
		sensor_data_ready_FLAG=FALSE;
			
		if(i<1000){
		//calc gyro offset with 1000 samples
			goffsetx+=local_data.gyro[0];
			goffsety+=local_data.gyro[1];
			goffsetz+=local_data.gyro[2];
			i++;
			if(i==1000){
				BSP_LED_On(LED0);
				goffsetx/=1000;
				goffsety/=1000;
				goffsetz/=1000;
			}
			}else{
				MadgwickAHRSupdate(q, 1.0f/SAMPLINGRATE,(local_data.gyro[0]-goffsetx)*coef,(local_data.gyro[1]-goffsety)*coef,(local_data.gyro[2]-goffsetz)*coef,local_data.acc[0],local_data.acc[1],local_data.acc[2],local_data.mag[1]*-1,local_data.mag[0]*-1,local_data.mag[2]);
			}
			sensor_print(NULL);
				
			//static char tmpbuf[30];
			/*memset(tmpbuf,0,30);
			sprintf(tmpbuf,"gyro %.2f %.2f %.2f  ",data->gyro[0],data->gyro[1],data->gyro[2]);
			UART_Transmit((uint8_t *)tmpbuf,30);
			memset(tmpbuf,0,30);
			sprintf(tmpbuf,"acc %.2f %.2f %.2f\n",data->acc[0],data->acc[1],data->acc[2]);
			UART_Transmit((uint8_t *)tmpbuf,30);
			memset(tmpbuf,0,30);
			sprintf(tmpbuf,"mag %.2f %.2f %.2f\n",data->mag[0],data->mag[1],data->mag[2]);
			UART_Transmit((uint8_t *)tmpbuf,30);*/
	}
}

