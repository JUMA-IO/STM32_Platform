#ifndef _IMU_SENSOR_FUSION_H_
#define _IMU_SENSOR_FUSION_H_
#include "cube_hal.h"
void complementary_filter(float acc_data[3], float gyr_data[3], float mag_data[3], float *pitch, float *roll, float *yal);


#endif /*_IMU_SENSOR_FUSION_H_*/

