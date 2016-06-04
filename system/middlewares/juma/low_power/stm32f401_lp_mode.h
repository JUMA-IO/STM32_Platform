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
#ifndef STM32F401_LP_MODE_H_
#define STM32F401_LP_MODE_H_

#ifdef RTC_LSI
	#define RTC_ASYNCH_PREDIV    0x1F
	#define RTC_SYNCH_PREDIV     0x0408
#endif

#ifdef RTC_LSE
	#define RTC_ASYNCH_PREDIV    0x1F
	#define RTC_SYNCH_PREDIV     0x03FF
#endif


/*RTC Init*/
uint32_t RTC_CalendarShow(uint8_t* showtime);
void rtc_init(void);
void rtc_wake_up_timer_config(uint32_t time);
void StandbyMode_Measure(void);
void StopMode_Measure(void);
void SleepMode_Measure(void);
void StandbyRTCBKPSRAMMode_Measure(void);
#endif //STM32F401_LP_MODE_H_



