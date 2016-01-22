#include "imu_sensor_fusion.h"
#include "imu_sensor.h"
#include "math.h"

#define GYROSCOPE_SENSITIVITY 15/10

#define M_PI 3.14159265359

#define dt 0.00125							// 1.25 ms sample rate!    

void complementary_filter(float acc_data[3], float gyr_data[3], float mag_data[3], float *pitch, float *roll, float *yal)
{
    float pitch_acc, roll_acc, yal_mag;

    // Integrate the gyroscope data -> int(angularSpeed) = angle
    *pitch += (gyr_data[0] / GYROSCOPE_SENSITIVITY) * dt; // Angle around the X-axis
    //*roll -= ((float)gyr_data[1] / GYROSCOPE_SENSITIVITY) * dt;    // Angle around the Y-axis
    *roll += (gyr_data[1] / GYROSCOPE_SENSITIVITY) * dt;    // Angle around the Y-axis
    *yal +=  (gyr_data[2] / GYROSCOPE_SENSITIVITY) * dt;    // Angle around the z-axis

    // Turning around the X axis results in a vector on the Y-axis
    pitch_acc = atan2f(acc_data[0], acc_data[2]) * 180 / M_PI;
    *pitch = *pitch * 0.98 + pitch_acc * 0.02;
    // Turning around the Y axis results in a vector on the X-axis
    roll_acc = atan2f(acc_data[1], (float)acc_data[2]) * 180 / M_PI;
    *roll = *roll * 0.98 + roll_acc * 0.02;

    yal_mag = atan2f(mag_data[0], mag_data[1]) * 180 / M_PI;
    *yal = *yal * 0.98 + yal_mag * 0.02;

}




