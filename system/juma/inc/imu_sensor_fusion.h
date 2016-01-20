#ifndef _IMU_SENSOR_FUSION_H_
#define _IMU_SENSOR_FUSION_H_
#include "cube_hal.h"
void complementary_filter(int32_t acc_data[3], int32_t gyr_data[3], int32_t mag_data[3], float *pitch, float *roll, float *yal);


#endif /*_IMU_SENSOR_FUSION_H_*/

