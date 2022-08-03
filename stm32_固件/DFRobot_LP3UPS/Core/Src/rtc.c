/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    rtc.c
  * @brief   This file provides code for the configuration
  *          of the RTC instances.
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
/* Includes ------------------------------------------------------------------*/
#include "rtc.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

RTC_HandleTypeDef hrtc;

/* RTC init function */
void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
  hrtc.Init.OutPut = RTC_OUTPUTSOURCE_ALARM;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

void HAL_RTC_MspInit(RTC_HandleTypeDef* rtcHandle)
{

  if(rtcHandle->Instance==RTC)
  {
  /* USER CODE BEGIN RTC_MspInit 0 */

  /* USER CODE END RTC_MspInit 0 */
    HAL_PWR_EnableBkUpAccess();
    /* Enable BKP CLK enable for backup registers */
    __HAL_RCC_BKP_CLK_ENABLE();
    /* RTC clock enable */
    __HAL_RCC_RTC_ENABLE();

    /* RTC interrupt Init */
    HAL_NVIC_SetPriority(RTC_IRQn, 4, 4);
    HAL_NVIC_EnableIRQ(RTC_IRQn);
    HAL_NVIC_SetPriority(RTC_Alarm_IRQn, 4, 4);
    HAL_NVIC_EnableIRQ(RTC_Alarm_IRQn);
  /* USER CODE BEGIN RTC_MspInit 1 */

  /* USER CODE END RTC_MspInit 1 */
  }
}

void HAL_RTC_MspDeInit(RTC_HandleTypeDef* rtcHandle)
{

  if(rtcHandle->Instance==RTC)
  {
  /* USER CODE BEGIN RTC_MspDeInit 0 */

  /* USER CODE END RTC_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_RTC_DISABLE();

    /* RTC interrupt Deinit */
    HAL_NVIC_DisableIRQ(RTC_IRQn);
    HAL_NVIC_DisableIRQ(RTC_Alarm_IRQn);
  /* USER CODE BEGIN RTC_MspDeInit 1 */

  /* USER CODE END RTC_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

__IO uint32_t TimeDisplay = 0;

struct uart_RTC_Time buff[20] = {0};

// 时钟结构体
_calendar_obj calendar, add_calendar;

//平年的月份日期表
const uint8_t mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};

/**
 * @brief 判断是否为闰年
 * 
 * @param nYear 待判断的年份
 * @return uint8_t 返回 1 代表闰年, 0 代表平年
 */
uint8_t Is_Leap_Year(uint16_t nYear)
{
  if ((nYear % 4U) != 0U)
  {
    return 0U;
  }

  if ((nYear % 100U) != 0U)
  {
    return 1U;
  }

  if ((nYear % 400U) == 0U)
  {
    return 1U;
  }
  else
  {
    return 0U;
  }
}

/**
 * @brief Get the Time Point
 * 
 * @param syear 年
 * @param smon 月
 * @param sday 日
 * @param hour 时
 * @param min 分
 * @param sec 秒
 * @return uint32_t 计算得到的时间点
 */
uint32_t getTimePoint(uint16_t syear ,uint16_t smon ,uint16_t sday ,uint16_t hour ,uint16_t min ,uint16_t sec)
{
  uint16_t t;
  uint32_t seccount=0;

  for(t=2000;t<syear;t++)  //把所有年份的秒钟相加
  {
    if(Is_Leap_Year(t))seccount+=31622400;//闰年的秒钟数
    else seccount+=31536000;        //平年的秒钟数
  }
  smon-=1;
  for(t=0;t<smon;t++)     //把前面月份的秒钟数相????
  {
    seccount+=(uint32_t)mon_table[t]*86400;//月份秒钟数相????
    if(Is_Leap_Year(syear)&&t==1)seccount+=86400;//闰年2月份增加????天的秒钟????
  }
  seccount+=(uint32_t)(sday-1)*86400;//把前面日期的秒钟数相????
  seccount+=(uint32_t)hour*3600;//小时秒钟????
  seccount+=(uint32_t)min*60;   //分钟秒钟????
  seccount+=sec;//????后的秒钟加上????

  return seccount;
}

/**
 * @brief 加日期, 往后增加相应日期
 * 
 * @param w_year 年
 * @param w_month 月
 * @param w_date 日
 * @param days 增加的天数
 */
void add_date(uint16_t *w_year ,uint16_t *w_month ,uint16_t *w_date ,uint8_t days)
{
  char Month_buf[13] = {0,31,28,31,30,31,30,31,31,30,31,30,31};               //月份修正????
  if(Is_Leap_Year(*w_year)) Month_buf[1] = 29;                //如果是闰年二????29

  switch((*w_month))
  {
    case 1:
      if((*w_date + days) > Month_buf[(*w_month)])
      {
        (*w_month)++;
        *w_date = *w_date + days - Month_buf[1];
      }else{
        *w_date += days;
      }

      break;
#if 1
    case 2:
      if((*w_date + days) > Month_buf[(*w_month)])
      {
        (*w_month)++;
        *w_date = *w_date + days - Month_buf[2];
      }else{
        *w_date += days;
      }
      break;
    case 3:
      if((*w_date + days) > Month_buf[(*w_month)])
      {
        (*w_month)++;
        *w_date = *w_date + days - Month_buf[3];
      }else{
        *w_date += days;
      }
      break;
    case 4:
      if((*w_date + days) > Month_buf[(*w_month)])
      {
        (*w_month)++;
        *w_date = *w_date + days - Month_buf[4];
      }else{
        *w_date += days;
      }
      break;
    case 5:
      if((*w_date + days) > Month_buf[(*w_month)])
      {
        (*w_month)++;
        *w_date = *w_date + days - Month_buf[5];
      }else{
        *w_date += days;
      }
      break;
    case 6:
      if((*w_date + days) > Month_buf[(*w_month)])
      {
        (*w_month)++;
        *w_date = *w_date + days - Month_buf[6];
      }else{
        *w_date += days;
      }
      break;
    case 7:
      if((*w_date + days) > Month_buf[(*w_month)])
      {
        (*w_month)++;
        *w_date = *w_date + days - Month_buf[7];
      }else{
        *w_date += days;
      }
      break;
    case 8:
      if((*w_date + days) > Month_buf[(*w_month-1)])
      {
        (*w_month)++;
        *w_date = *w_date + days - Month_buf[8];
      }else{
        *w_date += days;
      }
      break;
    case 9:
      if((*w_date + days) > Month_buf[(*w_month)])
      {
        (*w_month)++;
        *w_date = *w_date + days - Month_buf[9];
      }else{
        *w_date += days;
      }
      break;
    case 10:
      if((*w_date + days) > Month_buf[(*w_month)])
      {
        (*w_month)++;
        *w_date = *w_date + days - Month_buf[10];
      }else{
        *w_date += days;
      }
      break;
    case 11:
      if((*w_date + days) > Month_buf[(*w_month)])
      {
        (*w_month)++;
        *w_date = *w_date + days - Month_buf[11];
      }else{
        *w_date += days;
      }
      break;
    case 12:
      if((*w_date + days) > Month_buf[(*w_month)])
      {
        *w_year += 1;
        (*w_month) = 1;
        *w_date  = *w_date + days - Month_buf[12];
      }else{
        *w_date += days;
      }
      break;
    default:
      break;
#endif
  }
}

/**
 * @brief 校准日期，判断日期是否合法
 * 
 * @param w_year 年
 * @param w_month 月
 * @param w_date 日
 * @return uint8_t 返回 1 代表日期合法， 0 不合法
 */
uint8_t Check_date(uint16_t w_year ,uint8_t w_month ,uint8_t w_date)
{
  char Month_buf[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };   //月份修正????

  if (w_month==2)   //闰年2????+1????
    (((w_year%4==0)&&(w_year%100!=0))||(w_year%400==0))?Month_buf[1]+=1:Month_buf[1];

  if (w_month>12||w_month<1||w_date>Month_buf[w_month-1]||w_date<1)   //判断月份日期是否合法
    return 0;

  return 1;
}

/**
  * @brief  Gets the RTC counter value.
  * @param  None
  * @return RTC counter value.
  */
uint32_t RTC_GetCounter(void)
{
  uint16_t tmp = 0;
  tmp = RTC->CNTL;
  return (((uint32_t)RTC->CNTH << 16 ) | tmp) ;
}

/**
  * @brief  设置当前的时间
  * @param  sYear - 年, 2000~2099
  * @param  sMon - 月
  * @param  sDay - 日
  * @param  sHour - 时
  * @param  sMin - 分
  * @param  sSec - 秒
  * @return 成功返回0, 否则代表操作失败
  */
uint8_t RTC_Set(uint16_t sYear,uint16_t sMon ,uint16_t sDay ,uint16_t sHour ,uint16_t sMin ,uint16_t sSec)
{
  if(sYear<2000||sYear>2099) return 1;

  RTC_TimeTypeDef sTime = {sHour, sMin, sSec};
  // sTime.Hours = sHour;
  // sTime.Minutes = sMin;
  // sTime.Seconds = sSec;
  RTC_DateTypeDef sDate = {0U};
  sDate.Year = sYear - 2000;
  sDate.Month = sMon;
  sDate.Date = sDay;

  HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
  HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);

  return 0;
}

/**
  * @brief  得到当前的时间
  * @return 返回当前的时间, _nowTime
  */
_nowTime RTC_Get(void)
{
  _nowTime returnData;


  RTC_TimeTypeDef gTime = {0U};
  // RTC_DateTypeDef gDate = {0U};

  /* Call HAL_RTC_GetTime function to update date if counter higher than 24 hours */
  if (HAL_RTC_GetTime(&hrtc, &gTime, RTC_FORMAT_BIN) != HAL_OK)
  {
    printf("HAL_RTC_GetTime error!\n");
  }
  // HAL_RTC_GetDate(&hrtc, &gDate, RTC_FORMAT_BIN);

  /* Fill the structure fields with the read parameters */
  returnData.tm_oldTimePoint = hrtc.DateToUpdate.Tick;

  returnData.tm_year         = hrtc.DateToUpdate.Year + 2000;
  returnData.tm_month        = hrtc.DateToUpdate.Month;
  returnData.tm_date         = hrtc.DateToUpdate.Date;
  returnData.tm_week         = hrtc.DateToUpdate.WeekDay;

  returnData.tm_hour         = gTime.Hours;
  returnData.tm_minute       = gTime.Minutes;
  returnData.tm_second       = gTime.Seconds;

#if 0
  printf("returnData.tm_oldTimePoint = %u\n", returnData.tm_oldTimePoint);
  printf("returnData.tm_year = %u\n", returnData.tm_year);
  printf("returnData.tm_month = %u\n", returnData.tm_month);
  printf("returnData.tm_date = %u\n", returnData.tm_date);
  printf("returnData.tm_week = %u\n", returnData.tm_week);
  printf("returnData.tm_hour = %u\n", returnData.tm_hour);
  printf("returnData.tm_minute = %u\n", returnData.tm_minute);
  printf("returnData.tm_second = %u\n", returnData.tm_second);
#endif

  return returnData;
}

/* USER CODE END 1 */
