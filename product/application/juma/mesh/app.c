
#include "app.h"
#include "mesh.h"

#if NO_PRINTF
#define printf(...)
#endif

#define MESH_ID  0x0004

#ifdef CANNON_V2
char board_name[20] = "mesh_3";
#endif
#ifdef CANNON_V1
char board_name[20] = "CANNON_V1";
#endif

void on_ready(void)
{
    uint8_t tx_power_level = 5;
    uint16_t scan_interval = 400, adv_interval = 567;
    uint8_t bdAddr[6] = {0x04,0x02,0x04,0x03,0x07,0x0d };
    HCI_get_bdAddr(bdAddr);
    /*Config Adv Parameter And Ready to Adv*/
    ble_set_adv_param(board_name, bdAddr, tx_power_level, adv_interval);
    ble_device_start_advertising();
    /*Host*/
    ble_host_set_scan_param(tx_power_level, bdAddr, scan_interval);
    ble_host_start_scan(NULL);

}

void mesh_on_message(void* data)
{
    uint16_t* temp = data;
 
    if((*temp) & MESH_ID)
    {
        BSP_LED_On(LED0);
    }else{
        BSP_LED_Off(LED0);
    }
}

void ble_host_on_device_info(void* data)
{
    uint8_t* temp_buf = data;
#ifdef CLIENT_ROLE
    scan_device_found_info* device_info = data;

#endif
#if CLIENT_SERVER_ROLE
    if(temp_buf[10] == AD_TYPE_MANUFACTURER_SPECIFIC_DATA) {
        temp_buf = temp_buf+9;
        run_after_delay(mesh_rx_scan_data, temp_buf,2);

    }
#endif
}

void ble_host_on_message(uint8_t type, uint16_t length, uint8_t* value)
{

}

void ble_host_on_connect(void)
{
    printf("host connected \n");
}

void ble_device_on_message(uint8_t type, uint16_t length, uint8_t* value)
{

    mesh_manuf_data_t rx_data ;
    rx_data.data = (((uint16_t)value[0] << 8) | value[1]);
    printf("value: %x\n", rx_data.data);
    run_when_idle(mesh_rx_host_message, &rx_data);
}

void ble_device_on_connect( void )
{

}

void ble_device_on_disconnect(uint8_t reason)
{
   printf("device disconnected\n");
   run_after_delay(mesh_disconnect_handle, NULL, 100);
}



