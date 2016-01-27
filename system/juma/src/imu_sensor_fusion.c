#include "imu_sensor_fusion.h"
#include "imu_sensor.h"
#include "math.h"
#include "app.h"

#define GYROSCOPE_SENSITIVITY     4.375
#define ACCELERATOR_SENSITIVITY   0.061
#define GYRO_DEGREE_OFFSET        3.0
#define ACC_1_G                   1000.0

#define M_PI 3.1415926

#define dt 0.00125							// 1.25 ms sample rate!    

void complementary_filter(float acc_raw[3], float gyr_raw[3], float mag_raw[3], float *pitch, float *roll, float *yaw)
{
    float pitch_acc, roll_acc, yaw_mag;

    // Integrate the gyroscope data -> int(angularSpeed) = angle
    *pitch += gyr_raw[0] * dt; // Angle around the X-axis
    *roll -= gyr_raw[1] * dt;    // Angle around the Y-axis
    *yaw  += gyr_raw[2] * dt;
//    // Turning around the X axis results in a vector on the Y-axis
    pitch_acc = atan2f(acc_raw[0], acc_raw[2]) * 180 / M_PI;
    *pitch = *pitch * 0.98 + pitch_acc * 0.02;
    // Turning around the Y axis results in a vector on the X-axis
    roll_acc = atan2f(acc_raw[1], acc_raw[2]) * 180 / M_PI;
    *roll = *roll * 0.98 + roll_acc * 0.02;
    yaw_mag = atan2f(mag_raw[1], mag_raw[0]) * 180 / M_PI;
    *yaw = *yaw * 0.98 + yaw_mag * 0.02;
}

void imu_sensor_fusion_1(imu_sensor_data_t* sensor_raw, sensor_fusion_angle_t* sensor_angle,  imu_sensor_fusion_1_context_t* sensor_context)
{
    float k_acc, k_gyr, k_mag;
    float delta_gyr, delta_acc, delta_mag, delta_angle;
    float forceMagnitudeApprox, gyro_offset ;

    /*gyro factor*/
    forceMagnitudeApprox = sqrt(abs(sensor_raw->gyro[0]) * abs(sensor_raw->gyro[0]) +
                                abs(sensor_raw->gyro[1]) * abs(sensor_raw->gyro[1]) +
                                abs(sensor_raw->gyro[2]) * abs(sensor_raw->gyro[2]));

    gyro_offset = sqrt( (sensor_context->gyro_offset_x * sensor_context->gyro_offset_x) +
                        (sensor_context->gyro_offset_y * sensor_context->gyro_offset_y) +
                        (sensor_context->gyro_offset_z * sensor_context->gyro_offset_z));

    if(forceMagnitudeApprox > gyro_offset)
    {
        k_gyr = sensor_context->k_gyr_1;
    } else {
        k_gyr = sensor_context->k_gyr_2;
    }

    /*acc factor*/
    forceMagnitudeApprox = sqrt(abs(sensor_raw->acc[0]) * abs(sensor_raw->acc[0]) +
                                abs(sensor_raw->acc[1]) * abs(sensor_raw->acc[1]) +
                                abs(sensor_raw->acc[2]) * abs(sensor_raw->acc[2]));

    if((abs(forceMagnitudeApprox - ACC_1_G) / ACC_1_G) > 0.01 )
    {
        k_acc = sensor_context->k_acc_2;
    } else {
        k_acc = sensor_context->k_acc_1;
    }
    
    /*mag factor*/
    k_mag = sensor_context->k_mag_1;

    /*pitch*/
    sensor_raw->gyro[0] = sensor_raw->gyro[0] -  sensor_context->gyro_offset_x;
    delta_gyr =  sensor_raw->gyro[0] * dt;
    delta_acc = (atan2f(sensor_raw->acc[0], sensor_raw->acc[2]) * 180.0 / M_PI) - sensor_angle->pitch;
    delta_angle = k_gyr * delta_gyr + k_acc * delta_acc;
    sensor_context->gyro_offset_x += (   sensor_context->k_offset * (  ( sensor_raw->gyro[0] - (delta_angle / dt) ) -  sensor_context->gyro_offset_x  )   );
    sensor_angle->pitch += delta_angle;
    
    /*roll*/
    sensor_raw->gyro[1] = sensor_raw->gyro[1] - sensor_context->gyro_offset_y;
    delta_gyr =  sensor_raw->gyro[1] * dt;
    delta_acc = (atan2f(sensor_raw->acc[1], sensor_raw->acc[2]) * 180.0 / M_PI) - sensor_angle->roll;
    delta_angle = k_gyr * delta_gyr + k_acc * delta_acc;
    sensor_context->gyro_offset_y += (   sensor_context->k_offset * (  ( sensor_raw->gyro[1] - (delta_angle / dt) ) - sensor_context->gyro_offset_y  )   );
    sensor_angle->roll += delta_angle;

    /*yaw*/
    sensor_raw->gyro[2] = sensor_raw->gyro[2] - sensor_context->gyro_offset_z;
    delta_gyr =  sensor_raw->gyro[2] * dt;
    delta_mag = (atan2f(sensor_raw->mag[1], sensor_raw->mag[0]) * 180.0 / M_PI) - sensor_angle->yaw;
    delta_angle = k_gyr * delta_gyr + k_mag * delta_mag;
    sensor_context->gyro_offset_z += (   sensor_context->k_offset * (  ( sensor_raw->gyro[2] - (delta_angle / dt) ) - sensor_context->gyro_offset_z  )   );
    sensor_angle->yaw += delta_angle;
    
    
}



