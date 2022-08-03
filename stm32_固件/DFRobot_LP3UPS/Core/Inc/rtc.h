/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    rtc.h
  * @brief   This file contains all the function prototypes for
  *          the rtc.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __RTC_H__
#define __RTC_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

extern RTC_HandleTypeDef hrtc;

/* USER CODE BEGIN Private defines */
typedef struct rtc_time
{
  uint32_t tm_oldTimePoint;
  int tm_year;
  int tm_month;
  int tm_date;
  int tm_hour;
  int tm_minute;
  int tm_second;
  int tm_week;
}_nowTime;

//时间结构体
typedef struct
{
  volatile uint8_t hour;
  volatile uint8_t min;
  volatile uint8_t sec;
  //公历日月年周
  volatile uint16_t  w_year;
  volatile uint8_t  w_month;
  volatile uint8_t  w_date;
  volatile uint8_t  week;
}_calendar_obj;

struct uart_RTC_Time
{
	uint32_t timePoint;                  // 转成秒数  100  年的秒数

	uint16_t year;
	uint16_t  month;
	uint16_t  date;

	uint16_t  hour;
	uint16_t  minute;
	uint16_t  second;
	uint16_t  period;					// 循环的周??????

	uint16_t old_year;
	uint16_t  old_month;
	uint16_t  old_date;

	uint16_t  old_hour;
	uint16_t  old_minute;
	uint16_t  old_second;

	uint16_t  offset;				    // 周循环的位置
	uint16_t  type;						// AT指令类型
	uint16_t  number;				    // 闹钟的???数
	uint16_t  count;				        // 序列??????
	uint8_t  arr[7];
};

extern __IO uint32_t TimeDisplay;
extern _calendar_obj calendar, add_calendar;	//日历结构体
extern const uint8_t mon_table[12];	//月份日期数据表
extern struct uart_RTC_Time buff[20];
/* USER CODE END Private defines */

void MX_RTC_Init(void);

/* USER CODE BEGIN Prototypes */
uint32_t getTimePoint(uint16_t syear ,uint16_t smon ,uint16_t sday ,uint16_t hour ,uint16_t min ,uint16_t sec);
void BubbleSort(void);
void add_date(uint16_t *w_year ,uint16_t *w_month ,uint16_t *w_date ,uint8_t days);
uint8_t Check_date(uint16_t w_year ,uint8_t w_month ,uint8_t w_date);

uint32_t RTC_GetCounter(void);
_nowTime RTC_Get(void);
uint8_t RTC_Set(uint16_t sYear,uint16_t sMon ,uint16_t sDay ,uint16_t sHour ,uint16_t sMin ,uint16_t sSec);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __RTC_H__ */

