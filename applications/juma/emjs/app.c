
#include "app.h"
#include "bsp_common.h"
#include "bluenrg_sdk_api.h"
/*start adv*/
#include "mujs.h"


int errno;
void assert(s) {s;};
long time() {return 0;};
int signbit(double n) {return (n>0);};
int isfinite(double n) {return (n==-1);};
int isnan(double n) {return 0;};
int isinf(double n) {return !isfinite(n);};
char *name = "Remote Control";
uint8_t adv_address[6] = {0x08, 0x05, 0x04, 0x03, 0x02, 0x04};


#include "jsi.h"
#include "jsparse.h"
#include "jscompile.h"
#include "jsvalue.h"
#include "jsrun.h"
#include "jsbuiltin.h"

static const char *test_js =
    "hello('world');"
;

static void hello(js_State *J)
{
	const char *name = js_tostring(J, 1);
	static int invert = 0;

	if (invert > 0) {
		BSP_LED_On(LED0);
		invert --;
	} else {
		BSP_LED_Off(LED0);
		invert ++;
	}
	js_pushundefined(J);
}

void do_js_task()
{
    js_State *J;

    J = js_newstate(NULL, NULL, JS_STRICT);
	
    js_newcfunction(J, hello, "hello", 1);
    js_setglobal(J, "hello");
    js_dostring(J, test_js);
    js_freestate(J);
}

void on_ready(void)
{
 
    uint8_t tx_power_level = 7;
    uint16_t adv_interval = 100;
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
	static uint8_t i = 0;

    /*LED Control*/
    if(*value == 0x00) {
        BSP_LED_On(LED0);
    } else if(*value == 0x01) {
        BSP_LED_Off(LED0);
        do_js_task();
		*value = i++;
        ble_device_send(type, length, value);
    }

}

void out_of_mem()
{
}

/* Device on connect */
void ble_device_on_connect(void)
{


}
/* Device on disconnect */
void ble_device_on_disconnect(uint8_t reason)
{

}
