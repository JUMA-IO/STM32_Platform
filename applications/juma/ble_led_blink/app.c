//#include "sensor_service.h"

#include "app.h"
/*start adv*/

char *name = "BlueNRG_Blink";
uint8_t adv_address[] = {0x08, 0x05, 0x04, 0x03, 0x02, 0x04};	
uint8_t tx_power_level = 7;
uint16_t adv_interval = 100;
static float humidity;
static float temperature;
void LED_ON(void *args);
void LED_OFF(void *args);
void on_ready(void)
{
	/*Config Adv Parameter And Ready to Adv*/
	ble_set_adv_param(name, adv_address, tx_power_level, adv_interval);
	ble_device_start_advertising();
	run_after_delay(LED_ON,NULL,100);
}
void LED_ON(void *args){
	BSP_LED_On(LED0);
	run_after_delay(LED_OFF,NULL,250);
}
void LED_OFF(void *args){
	BSP_LED_Off(LED0);
	run_after_delay(LED_ON,NULL,750);
}

