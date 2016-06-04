#ifndef _APP_H_
#define _APP_H_


extern void hum_temp_monitor_init(void);
extern void pressure_sensor_init(void);
extern void lsm6ds3_6_axis_init(void);
extern void lsm303agr_init(void);
extern void read_temp_hum(void* args);
extern void read_pressure(void* args);
extern void read_6_Axis_data(void* args);
extern void read_raw_magnetometer_data(void* args);

#endif //_APP_H_



