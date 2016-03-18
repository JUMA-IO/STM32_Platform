#include "mesh.h"

#if NO_PRINTF
#define printf(...)
#endif

#define LED_ON 0x0001
#define LED_OFF 0x0002

uint16_t adv_info_number = 1;

/*manufacturing data in advertise payload*/
static mesh_manuf_data_t  mesh_manuf_data = {
    9,                                  //length
    AD_TYPE_MANUFACTURER_SPECIFIC_DATA,  //adv data type
    0x0001,                              //advertise information number
    MESH_GROUP_ID,                       // mesh group id
    MESH_ID,                             // mesh id
    0x0000                               // data
};

static void mesh_manuf_data_config(void* data)
{
     mesh_manuf_data_t* rx_data = data;
     
     adv_info_number++;
     mesh_manuf_data.adv_info_number = adv_info_number;
     mesh_manuf_data.group_id = rx_data->group_id;
     mesh_manuf_data.id = rx_data->id;
     mesh_manuf_data.data = rx_data->data;
     printf("mesh_manuf_data_config:data:%x,%x,%x\n", mesh_manuf_data.data, mesh_manuf_data.group_id ,mesh_manuf_data.id);
}

void mesh_rx_scan_data(void* data)
{
      mesh_manuf_data_t* rx_data = data;
  
    if(rx_data->adv_info_number > adv_info_number){
      
       if((rx_data->group_id == 0x0000)  || (((rx_data->id & MESH_ID) || (rx_data->id == 0x0000)) && (rx_data->group_id & MESH_GROUP_ID))){
          if(rx_data->data & LED_ON){
             BSP_LED_On(LED0);
          }
          if(rx_data->data & LED_OFF){
             BSP_LED_Off(LED0);
          }
       }
       run_when_idle(mesh_manuf_data_config, data);
       run_when_idle(mesh_adv_data_update, NULL);
    }
}

void mesh_rx_host_message(void* data)
{
    mesh_manuf_data_t* rx_data = data;
 
     if((rx_data->group_id == 0x0000)  || (((rx_data->id & MESH_ID) || (rx_data->id == 0x0000)) && (rx_data->group_id & MESH_GROUP_ID))){
      if(rx_data->data & LED_ON){
         BSP_LED_On(LED0);
      }
      if(rx_data->data & LED_OFF){
         BSP_LED_Off(LED0);
      }
   }
    
    run_when_idle(mesh_manuf_data_config, data);
}

/*set manufacturing data on the advertising packet*/
void mesh_adv_data_update(void* arg)
{
    tBleStatus ret;   
    ret = aci_gap_update_adv_data(10, (uint8_t*)&mesh_manuf_data);
    if(ret){
        printf("aci_gap_update_adv_data failed \n");
    }
 
}

void mesh_disconnect_handle(void* data)
{
    tBleStatus ret;
 
    ret = ble_host_stop_scan();
    if(ret){
       printf("ble_host_stop_scan failed \n");
    }
    
    ret = ble_device_start_advertising();
    if(ret){
       printf("ble_device_start_advertising \n");
    }
    
    run_when_idle(mesh_adv_data_update, NULL);
}









