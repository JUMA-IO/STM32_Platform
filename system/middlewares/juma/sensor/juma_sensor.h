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
  
#ifndef JUMA_SENSOR_H_
#define JUMA_SENSOR_H_

#define JSENSOR_TYPE_HUMITY_TEMP		0x01
#define JSENSOR_TYPE_MOTION_6AXIS		0x02
#define JSENSOR_TYPE_PRESSURE			0x03
#define JSENSOR_TYPE_MAGNET			0x04

enum _JSensor_Status {
	JSENSOR_OK = 0,
	JSENSOR_FAIL = 1,
};

struct _JSensor_HUM_TEMP_data {
	int16_t *humidity;
	int16_t *temperature;
};

struct _JSensor_Press_data {
	int32_t *pressure;
};

struct _JSensor_AXIS_data {
	int8_t *ACC;
	int8_t *GRO;
};
struct _JSensor_MAG_data{
	int8_t *MAG;
};

typedef enum _JSensor_Status JSensor_Status;
typedef struct _JSensor_HUM_TEMP_data JSensor_HUM_TEMP_Typedef;
typedef struct _JSensor_Press_data JSensor_Press_Typedef;
typedef struct _JSensor_AXIS_data JSensor_AXIS_Typedef;
typedef struct _JSensor_MAG_data JSensor_MAG_Typedef;

void jsensor_sys_init(void);
void jsensor_app_set_sensors(void);
void jsensor_app_set_sensor(uint16_t sid);
JSensor_Status jsensor_app_read_sensor(uint16_t sid, void *data);

#endif //JUMA_SENSOR_H_



