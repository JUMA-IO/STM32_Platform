/*
 *
 *  JUMA.IO - JUMA SDK for STM families
 *
 *  Copyright (C) 2013-2016  JUMA Technology
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the Apache V2 License as published by
 *  the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 */
#ifndef _IMU_SENSOR_H_
#define _IMU_SENSOR_H_
/*sensor feature*/
#define IMU_SENSOR_FEATURE_ACC  0x01
#define IMU_SENSOR_FEATURE_GYRO 0x02
#define IMU_SENSOR_FEATURE_MAG  0x04

/*fifo threthold level*/
#define FIFO_WATER_MARK         6

/*FIFO CTRL MASK*/
#define LSM6DS3_XG_FIFO_THRESHOLD_MASK       ((uint8_t)0x0F)
#define LSM6DS3_XG_FIFO_INT_THRESHOLD_MASK   ((uint8_t)0x08)

/* Six axes sensor IO functions */
extern IMU_6AXES_StatusTypeDef LSM6DS3_IO_Init( void );
extern IMU_6AXES_StatusTypeDef LSM6DS3_IO_Write( uint8_t* pBuffer, uint8_t DeviceAddr, uint8_t RegisterAddr,
    uint16_t NumByteToWrite );
extern IMU_6AXES_StatusTypeDef LSM6DS3_IO_Read( uint8_t* pBuffer, uint8_t DeviceAddr, uint8_t RegisterAddr,
    uint16_t NumByteToRead );
extern void LSM6DS3_IO_ITConfig( void );

enum _imu_status{
   imu_status_ok = 0,
   imu_status_fail = 1,
};

typedef enum 
{
   OUTPUT_DISABLE = 0, 
   OUTPUT_ENABLE = !OUTPUT_DISABLE
}output_state;

typedef enum _sensor_selection{
   ACC_ENABLE           = 1,
   GYRO_ENABLE          = 2,
   ACC_AND_GYRO_ENABLE  = 3,
   MAG_ENABLE           = 4,
   ALL_ENABLE           = 7,
} sensor_selsection_t;

typedef enum sensor_data_type{
   TYPE_ACC_DATA  = 0,
   TYPE_GYRO_DATA = 1,
   TYPE_MAG_DATA  = 2,
} sensor_data_type_t;
typedef struct _imu_sensor_data_t {
    float acc[3];
    float gyro[3];
    float mag[3];
} imu_sensor_data_t;

typedef enum _imu_status imu_status_t;
typedef struct _imu_sensor_data_t imu_sensor_data_t; 

/*IMU Sensor API */
imu_status_t imu_sensor_9_axis_reset(void);
imu_status_t imu_sensor_9_axis_select_features(sensor_selsection_t features);
imu_status_t imu_sensor_9_axis_set_data_rate(uint32_t* p_data_rate, uint8_t mode); 
imu_status_t imu_sensor_9_axis_start(void); 
imu_status_t imu_sensor_9_axis_stop(void);
void imu_sensor_9_axis_read_data_from_fifo(void);

#endif /*_IMU_SENSOR_H_*/
