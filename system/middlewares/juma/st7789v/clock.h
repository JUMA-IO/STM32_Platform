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
#ifndef __CLOCK_H__
#define __CLOCK_H__

extern const uint32_t CLOCK_SECOND;
typedef uint32_t tClockTime;

void Clock_Init(void);
tClockTime Clock_Time(void);
void Clock_Wait(uint32_t i);
void Clock_Suspend(void);

#endif /* __CLOCK_H__ */

