#ifndef _APP_H_
#define _APP_H_
#include "bluenrg_sdk_api.h"
#include "x_nucleo_iks01a1.h"
#include "x_nucleo_iks01a1_hum_temp.h"
#include "x_nucleo_iks01a1_pressure.h"
#include "x_nucleo_iks01a1_imu_6axes.h"
#include "hts221.h"
#include "lps25hb.h"
#include "lps25h.h"
#include "lsm6ds3.h"
#include "juma_sensor.h"



extern void hum_temp_monitor_init(void);
extern void pressure_sensor_init(void);
extern void lsm6ds3_6_axis_init(void);
extern void lsm303agr_init(void);
extern void read_temp_hum(void* args);
extern void read_pressure(void* args);
extern void read_6_Axis_data(void* args);
extern void read_raw_magnetometer_data(void* args);

#endif //_APP_H_



