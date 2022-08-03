/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : usbd_cdc_if.c
  * @version        : v2.0_Cube
  * @brief          : Usb device for Virtual Com Port.
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
#include "usbd_cdc_if.h"
#include "rtc.h"
#include "i2c.h"
#include "battery.h"
#include <string.h>

/* USER CODE BEGIN INCLUDE */

/* USER CODE END INCLUDE */

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

volatile uint8_t uartLock     = 0;
uint8_t uartFlag     = 0;
uint8_t FlashWriteFlag     = 2;
uint8_t UTART_ANALYSIS[257] = {0}; 
uint8_t uartDataLen = 0;

uint8_t rtcDataLen  = 0;

uint16_t oldYear  = 0;
uint16_t oldMonth = 0;
uint16_t oldDate  = 0;

uint16_t now_time_year = 0;
uint8_t  now_time_month = 0;
uint8_t  now_time_date = 0;
uint8_t  now_time_hour = 0;
uint8_t  now_time_minute = 0;
uint8_t  now_time_second = 0;

char usbStr[100] = "";

_nowTime _time;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @brief Usb device library.
  * @{
  */

/** @addtogroup USBD_CDC_IF
  * @{
  */

/** @defgroup USBD_CDC_IF_Private_TypesDefinitions USBD_CDC_IF_Private_TypesDefinitions
  * @brief Private types.
  * @{
  */

/* USER CODE BEGIN PRIVATE_TYPES */

/* USER CODE END PRIVATE_TYPES */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Private_Defines USBD_CDC_IF_Private_Defines
  * @brief Private defines.
  * @{
  */

/* USER CODE BEGIN PRIVATE_DEFINES */
/* USER CODE END PRIVATE_DEFINES */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Private_Macros USBD_CDC_IF_Private_Macros
  * @brief Private macros.
  * @{
  */

/* USER CODE BEGIN PRIVATE_MACRO */

/* USER CODE END PRIVATE_MACRO */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Private_Variables USBD_CDC_IF_Private_Variables
  * @brief Private variables.
  * @{
  */
/* Create buffer for reception and transmission           */
/* It's up to user to redefine and/or remove those define */
/** Received data over USB are stored in this buffer      */
uint8_t UserRxBufferFS[APP_RX_DATA_SIZE];

/** Data to send over USB CDC are stored in this buffer   */
uint8_t UserTxBufferFS[APP_TX_DATA_SIZE];

/* USER CODE BEGIN PRIVATE_VARIABLES */
RxDef USB_COM;

/* USER CODE END PRIVATE_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Exported_Variables USBD_CDC_IF_Exported_Variables
  * @brief Public variables.
  * @{
  */

extern USBD_HandleTypeDef hUsbDeviceFS;

/* USER CODE BEGIN EXPORTED_VARIABLES */

/* USER CODE END EXPORTED_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Private_FunctionPrototypes USBD_CDC_IF_Private_FunctionPrototypes
  * @brief Private functions declaration.
  * @{
  */

static int8_t CDC_Init_FS(void);
static int8_t CDC_DeInit_FS(void);
static int8_t CDC_Control_FS(uint8_t cmd, uint8_t* pbuf, uint16_t length);
static int8_t CDC_Receive_FS(uint8_t* pbuf, uint32_t *Len);

/* USER CODE BEGIN PRIVATE_FUNCTIONS_DECLARATION */

/* USER CODE END PRIVATE_FUNCTIONS_DECLARATION */

/**
  * @}
  */

USBD_CDC_ItfTypeDef USBD_Interface_fops_FS =
{
  CDC_Init_FS,
  CDC_DeInit_FS,
  CDC_Control_FS,
  CDC_Receive_FS
};

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Initializes the CDC media low layer over the FS USB IP
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_Init_FS(void)
{
  /* USER CODE BEGIN 3 */
  /* Set Application Buffers */
  USBD_CDC_SetTxBuffer(&hUsbDeviceFS, UserTxBufferFS, 0);
  USBD_CDC_SetRxBuffer(&hUsbDeviceFS, UserRxBufferFS);
  return (USBD_OK);
  /* USER CODE END 3 */
}

/**
  * @brief  DeInitializes the CDC media low layer
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_DeInit_FS(void)
{
  /* USER CODE BEGIN 4 */
  return (USBD_OK);
  /* USER CODE END 4 */
}

/**
  * @brief  Manage the CDC class requests
  * @param  cmd: Command code
  * @param  pbuf: Buffer containing command data (request parameters)
  * @param  length: Number of data to be sent (in bytes)
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_Control_FS(uint8_t cmd, uint8_t* pbuf, uint16_t length)
{
  /* USER CODE BEGIN 5 */
  switch(cmd)
  {
    case CDC_SEND_ENCAPSULATED_COMMAND:

    break;

    case CDC_GET_ENCAPSULATED_RESPONSE:

    break;

    case CDC_SET_COMM_FEATURE:

    break;

    case CDC_GET_COMM_FEATURE:

    break;

    case CDC_CLEAR_COMM_FEATURE:

    break;

  /*******************************************************************************/
  /* Line Coding Structure                                                       */
  /*-----------------------------------------------------------------------------*/
  /* Offset | Field       | Size | Value  | Description                          */
  /* 0      | dwDTERate   |   4  | Number |Data terminal rate, in bits per second*/
  /* 4      | bCharFormat |   1  | Number | Stop bits                            */
  /*                                        0 - 1 Stop bit                       */
  /*                                        1 - 1.5 Stop bits                    */
  /*                                        2 - 2 Stop bits                      */
  /* 5      | bParityType |  1   | Number | Parity                               */
  /*                                        0 - None                             */
  /*                                        1 - Odd                              */
  /*                                        2 - Even                             */
  /*                                        3 - Mark                             */
  /*                                        4 - Space                            */
  /* 6      | bDataBits  |   1   | Number Data bits (5, 6, 7, 8 or 16).          */
  /*******************************************************************************/
    case CDC_SET_LINE_CODING:

    break;

    case CDC_GET_LINE_CODING:

    break;

    case CDC_SET_CONTROL_LINE_STATE:

    break;

    case CDC_SEND_BREAK:

    break;

  default:
    break;
  }

  return (USBD_OK);
  /* USER CODE END 5 */
}

/**
  * @brief  Data received over USB OUT endpoint are sent over CDC interface
  *         through this function.
  *
  *         @note
  *         This function will issue a NAK packet on any OUT packet received on
  *         USB endpoint until exiting this function. If you exit this function
  *         before transfer is complete on CDC interface (ie. using DMA controller)
  *         it will result in receiving more data while previous ones are still
  *         not sent.
  *
  * @param  Buf: Buffer of data to be received
  * @param  Len: Number of data received (in bytes)
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_Receive_FS(uint8_t* Buf, uint32_t *Len)
{
  /* USER CODE BEGIN 6 */
  USBD_CDC_SetRxBuffer(&hUsbDeviceFS, &Buf[0]);
  USBD_CDC_ReceivePacket(&hUsbDeviceFS);
  USBD_CDC_HandleTypeDef   *hcdc = (USBD_CDC_HandleTypeDef *) hUsbDeviceFS.pClassData;

  if(uartLock == 0)
  {
    USB_COM.RecLen=*Len;  // 添加自己
    USB_COM.RecFlag=1;    // 添加自己
    if(Buf[*Len-1] == '\r'){
      memcpy(UTART_ANALYSIS, hcdc->RxBuffer, *Len);
    }
  } else {
    USB_SEND_DATA("AT_BUSY\n");
  }

  return (USBD_OK);
  /* USER CODE END 6 */
}

/**
  * @brief  CDC_Transmit_FS
  *         Data to send over USB IN endpoint are sent over CDC interface
  *         through this function.
  *         @note
  *
  *
  * @param  Buf: Buffer of data to be sent
  * @param  Len: Number of data to be sent (in bytes)
  * @retval USBD_OK if all operations are OK else USBD_FAIL or USBD_BUSY
  */
uint8_t CDC_Transmit_FS(uint8_t* Buf, uint16_t Len)
{
  uint8_t result = USBD_OK;
  /* USER CODE BEGIN 7 */
  USBD_CDC_HandleTypeDef *hcdc = (USBD_CDC_HandleTypeDef*)hUsbDeviceFS.pClassData;
  // if (hcdc->TxState != 0){
  //   return USBD_BUSY;
  // }
  while (hcdc->TxState != 0);
  USBD_CDC_SetTxBuffer(&hUsbDeviceFS, Buf, Len);
  result = USBD_CDC_TransmitPacket(&hUsbDeviceFS);
  /* USER CODE END 7 */
  return result;
}

/* USER CODE BEGIN PRIVATE_FUNCTIONS_IMPLEMENTATION */

#if 1
/**
 * @brief 计算发送usb-cdc数据的长度，并完成数据发送
 * 
 * @param str 待发送的数据
 */
void USB_SEND_DATA(char * str)
{
  uint16_t len = strlen(str);
  // printf("len = %u\n", len);
  CDC_Transmit_FS((uint8_t *)str, len);
}

/**
 * @brief 刷新闹钟，刷新存储任务的数组
 * 
 */
void flush_Alarm(void)
{
  #if 0
    _nowTime time = RTC_Get();
    now_time_year = _time.tm_year;
    now_time_month = _time.tm_month;
    now_time_date = _time.tm_date;
    now_time_hour = _time.tm_hour;
    now_time_minute = _time.tm_minute;
    now_time_second = _time.tm_second;

    //DBG("year = %d, month = %d ,date = %d ,hour = %d, minute = %d, second = %d\n",now_time_year,now_time_month,now_time_date,now_time_hour,now_time_minute,now_time_second);
    if(time.tm_year == 0){
      // 时间错误
    }else{
      _time = time;
    }

    if(oldYear != _time.tm_year || oldMonth != _time.tm_month || oldDate != _time.tm_date)
    {
      if(buff[0].year == _time.tm_year && buff[0].month == _time.tm_month && buff[0].date == _time.tm_date)
      {
        RTC_Set_Alam(buff[0].year ,buff[0].month ,buff[0].date ,buff[0].hour ,buff[0].minute ,buff[0].second);
        // setAlarmTime();
        //printf("update alarm time\n");
      }
      oldYear  = _time.tm_year;
      oldMonth = _time.tm_month;
      oldDate  = _time.tm_date;
    }
  #endif
    _time = RTC_Get();
    if(oldYear != _time.tm_year || oldMonth != _time.tm_month || oldDate != _time.tm_date)
    {
      // if(buff[0].year == _time.tm_year && buff[0].month == _time.tm_month && buff[0].date == _time.tm_date)
      // {
        RTC_Set_Alam(buff[0].year ,buff[0].month ,buff[0].date ,buff[0].hour ,buff[0].minute ,buff[0].second);
        // setAlarmTime();
        //printf("update alarm time\n");
      // }
      oldYear  = _time.tm_year;
      oldMonth = _time.tm_month;
      oldDate  = _time.tm_date;
    }

}

/**
 * @brief 设置RTC闹钟
 * 
 * @param syear 年
 * @param smon 月
 * @param sday 日
 * @param hour 时
 * @param min 分
 * @param sec 秒
 * @return uint8_t 返回操作结果
 */
uint8_t RTC_Set_Alam(uint16_t syear,uint16_t smon,uint16_t sday,uint16_t hour,uint16_t min,uint16_t sec)
{
  _nowTime _time = RTC_Get();
  if(buff[0].year == _time.tm_year && buff[0].month == _time.tm_month && buff[0].date == _time.tm_date)
  {
    if(syear<2000||syear>2099) return 1;
    RTC_AlarmTypeDef sAlamTime = {0U};
    sAlamTime.AlarmTime.Seconds += (uint32_t)hour * 3600;   // 小时秒钟????
    sAlamTime.AlarmTime.Seconds += (uint32_t)min * 60;   // 分钟秒钟????
    sAlamTime.AlarmTime.Seconds += sec;   // ????后的秒钟加上????
    if(sAlamTime.AlarmTime.Seconds > _time.tm_oldTimePoint){
      HAL_RTC_SetAlarm_IT(&hrtc, &sAlamTime, RTC_FORMAT_BIN);
    }else{
      updateAlarm(_time.tm_week ,0,0,0,0,0,0,0);
      RTC_Set_Alam(buff[0].year ,buff[0].month ,buff[0].date ,buff[0].hour ,buff[0].minute ,buff[0].second);
    }
  }
  return 0;
}

/**
 * @brief 更新闹钟，更新闹钟的任务列表（数组）
 * 
 * @param Week 周
 * @param flag 循环任务 刷新标志
 * @param year 年
 * @param month 月
 * @param date 日
 * @param hour 时
 * @param minute 分
 * @param second 秒
 */
void updateAlarm(uint8_t Week ,uint8_t flag ,uint16_t year ,uint16_t month ,uint16_t date ,uint16_t hour ,uint16_t minute ,uint16_t second)
{
  
  switch(buff[0].period)
  {
    case PERIOD_ONCE:
      leftMove();
      rtcDataLen--;
      BubbleSort();        //排序
      break;
    case PERIOD_ONE_YEAR:
      // 年份 加一 重新排序
      buff[0].year++;
      while(0 == Check_date(buff[0].year ,buff[0].month ,buff[0].date))
      {
        buff[0].year++;
      }
      buff[0].timePoint = getTimePoint(buff[0].year ,buff[0].month ,buff[0].date ,buff[0].hour ,buff[0].minute ,buff[0].second);
      BubbleSort();        //排序
      break;
    case PERIOD_ONE_MONTH:
      // 月份 加一 重新排序
      if(buff[0].month == 12){
        buff[0].year++;
        buff[0].month = 1;
        while(0 == Check_date(buff[0].year ,buff[0].month ,buff[0].date))
        {
          if(buff[0].month == 12){
            buff[0].year++;
            buff[0].month = 1;
          }else{
            buff[0].month++;
          }
        }
        buff[0].timePoint = getTimePoint(buff[0].year ,buff[0].month ,buff[0].date ,buff[0].hour ,buff[0].minute ,buff[0].second);

      }else{
        buff[0].month++;
        while(0 == Check_date(buff[0].year ,buff[0].month ,buff[0].date))
        {
          if(buff[0].month == 12){
            buff[0].year++;
            buff[0].month = 1;
          }else{
            buff[0].month++;
          }
        }
        buff[0].timePoint = getTimePoint(buff[0].year ,buff[0].month ,buff[0].date ,buff[0].hour ,buff[0].minute ,buff[0].second);
      }
      BubbleSort();  //排序
      //printf("MMM ----%d/%d/%d/%d:%d:%d--\n ",buff[0].year ,buff[0].month ,buff[0].date ,buff[0].hour ,buff[0].minute ,buff[0].second);
      break;
    case PERIOD_TWO_WEEK:
      add_date(&buff[0].year ,&buff[0].month ,&buff[0].date ,14);
      buff[0].timePoint = getTimePoint(buff[0].year ,buff[0].month ,buff[0].date ,buff[0].hour ,buff[0].minute ,buff[0].second);
      BubbleSort();                                                                                       //排序
      break;
    case PERIOD_ONE_WEEK:
      add_date(&buff[0].year ,&buff[0].month ,&buff[0].date ,7);
      buff[0].timePoint = getTimePoint(buff[0].year ,buff[0].month ,buff[0].date ,buff[0].hour ,buff[0].minute ,buff[0].second);
      BubbleSort();                                                                                       //排序
      break;
    case PERIOD_ONE_DATE:
      add_date(&buff[0].year ,&buff[0].month ,&buff[0].date ,1);
      buff[0].timePoint = getTimePoint(buff[0].year ,buff[0].month ,buff[0].date ,buff[0].hour ,buff[0].minute ,buff[0].second);
      BubbleSort();                                                                                       //排序
      break;
    case PERIOD_ALARM:
      if(flag == 1){
        // uint8_t count = 0;
        uint8_t periodCount =  0;
        uint8_t ii          =  0;
        buff[0].year   = year;
        buff[0].month  = month;
        buff[0].date   = date;
        //printf("week = %d \n",Week);
        for(ii = 0; ii < buff[0].number; ii++)
        {
          if(buff[0].arr[ii] > Week){
            periodCount = 1;                  //设置星期大于等于当前星期
            //ii++;
            break;
          }else if(buff[0].arr[ii] == Week){
            periodCount = 2;
            //ii++;
            break;
          }else{
            periodCount = 0;
          }
        }
        switch(periodCount)
        {
          case 0:
            // 设置星期小于当前星期
            ii = 0;
            //printf("xiao yu %d \n",ii);
            add_date(&buff[0].year ,&buff[0].month ,&buff[0].date ,7 - Week + buff[0].arr[0]);
            break;
          case 1:
            //printf("da yu  %d \n",ii);
            add_date(&buff[0].year ,&buff[0].month ,&buff[0].date ,buff[0].arr[ii] - Week);
            break;
          case 2:
            //printf("deng yu %d \n",ii);
            if(buff[0].hour > hour){
              // 当天
            }else if(buff[0].hour == hour){
              if(buff[0].minute > minute){
                // 当天
              }else if(buff[0].minute == minute){
                if(buff[0].second > second){
                  // 当天
                }else{
                  if(ii + 1 > buff[0].number)
                  {
                    ii = 0;
                    add_date(&buff[0].year ,&buff[0].month ,&buff[0].date ,7 - Week + buff[0].arr[ii]);
                  }else{
                    ii++;
                    add_date(&buff[0].year ,&buff[0].month ,&buff[0].date ,buff[0].arr[ii] - Week);
                  }
                }
              }else{
                if(ii + 1 > buff[0].number)
                {
                  ii = 0;
                  add_date(&buff[0].year ,&buff[0].month ,&buff[0].date ,7 -Week + buff[0].arr[ii]);
                }else{
                  ii++;
                  add_date(&buff[0].year ,&buff[0].month ,&buff[0].date ,buff[0].arr[ii] - Week);
                }
              }
            }else{
              if(ii + 1 > buff[0].number)
              {
                ii = 0;
                add_date(&buff[0].year ,&buff[0].month ,&buff[0].date ,7 - Week + buff[0].arr[ii]);
              }else{
                ii++;
                add_date(&buff[0].year ,&buff[0].month ,&buff[0].date ,buff[0].arr[ii] - Week);
              }
            }
        }
        buff[0].timePoint = getTimePoint(buff[0].year ,buff[0].month ,buff[0].date ,buff[0].hour ,buff[0].minute ,buff[0].second);
        buff[0].offset = ii;
        //printf("******%d/%d/%d/%d:%d:%d- offset=%d\n",buff[0].year ,buff[0].month ,buff[0].date ,buff[0].hour ,buff[0].minute ,buff[0].second ,buff[0].arr[ii]);
      }else{    
        buff[0].offset++;
        if(buff[0].offset == buff[0].number)
        {
          buff[0].offset = 0;
        }
        if(buff[0].arr[buff[0].offset] >= Week){
          add_date(&buff[0].year ,&buff[0].month ,&buff[0].date ,buff[0].arr[buff[0].offset] - Week);
        }else{
          add_date(&buff[0].year ,&buff[0].month ,&buff[0].date ,7 - Week + buff[0].arr[buff[0].offset]);
        }
        buff[0].timePoint = getTimePoint(buff[0].year ,buff[0].month ,buff[0].date ,buff[0].hour ,buff[0].minute ,buff[0].second);
      }
      BubbleSort();
      break;
    case PERIOD_ALARM_ONCE:
      leftMove();
      rtcDataLen--;
      BubbleSort();        //排序
      break;
    default:
      break;
  }
  if(0 == FlashWriteFlag) {   // 任务列表改变，写入flash
    FlashWriteFlag = 1;
  }
  //setAlarmTime();
  //printf("end ----%d/%d/%d/%d:%d:%d--\n ",buff[0].year ,buff[0].month ,buff[0].date ,buff[0].hour ,buff[0].minute ,buff[0].second);
}

/**
 * @brief 删除闹钟，按任务序号
 * 
 * @param number 任务序号
 * @return uint8_t 操作状态
 */
uint8_t deleteAlarm(uint8_t number)
{
  if(number > rtcDataLen){
    return 0;
  }else{
    memset(&buff[number-1] ,0 ,sizeof(buff[0]));
    for(uint8_t i = number-1; i < rtcDataLen-1; i++)
    {
      memcpy(&buff[i]   ,&buff[i+1] ,sizeof(buff[0]));
    }

    memset(&buff[rtcDataLen-1] ,0 ,sizeof(buff[0]));
    rtcDataLen--;
  }
  if(0 == FlashWriteFlag) {   // 任务列表改变，写入flash
    FlashWriteFlag = 1;
  }
  return 1;
}

/**
 * @brief 左移任务列表
 * 
 */
void leftMove(void)
{
  for (uint8_t i = 0; i < rtcDataLen-1; i++)
  {
    memcpy(&buff[i]   ,&buff[i+1] ,sizeof(buff[0]));
  }
  memset(&buff[rtcDataLen-1] ,0 ,sizeof(buff[0]));
}

/**
 * @brief 新建闹钟任务
 * 
 * @param sTimePoint 时间刻度
 * @param sYear 年
 * @param sMonth 月
 * @param sDate 日
 * @param sHour 时
 * @param sMinute 分
 * @param sSecond 秒
 * @param sType 任务类型
 * @param sPeriod 循环周期
 * @param sOffset 偏移
 * @param sNumber 任务序号
 * @param sWeek 循环星期
 * @param sWeekLen 星期长度
 */
void Assignment(uint32_t sTimePoint ,uint16_t sYear ,uint8_t sMonth ,uint8_t sDate ,uint8_t sHour ,uint8_t sMinute ,uint8_t sSecond ,uint8_t sType ,uint8_t sPeriod ,uint8_t sOffset ,uint8_t sNumber ,uint8_t *sWeek ,uint8_t sWeekLen)
{
  for(uint8_t i=0; i<rtcDataLen; i++){
    printf(" 1 \n");
                  printf("sTimePoint = %u\n", sTimePoint);
                  printf("buff[%u].timePoint = %u\n", i, buff[i].timePoint);
    if(sTimePoint == buff[i].timePoint) {
    printf(" 2 \n");
      if(sType == buff[i].type) {
    printf(" 3 \n");
        if(sPeriod == buff[i].period) {
    printf(" 4 \n");
          if(sOffset == buff[i].offset) {
    printf(" 5 \n");
            if(sNumber == buff[i].number){
    printf(" 6 \n");
              if(sNumber){
                for(uint8_t j=0; j<sWeekLen; j++){
                  if(buff[i].arr[j] != sWeek[j]){
                    break;
                  }else{
                    if(j == (sWeekLen-1)){
                      AT_ALARM_REPEAT;
                      return;
                    }
                  }
                }
              }else{
                AT_ALARM_REPEAT;
                return;
              }
            }
          }
        }
      }
    }
  }

  buff[rtcDataLen].timePoint = sTimePoint;
  buff[rtcDataLen].year      = sYear;
  buff[rtcDataLen].month     = sMonth;
  buff[rtcDataLen].date      = sDate;
  buff[rtcDataLen].hour      = sHour;
  buff[rtcDataLen].minute    = sMinute;
  buff[rtcDataLen].second    = sSecond;

  buff[rtcDataLen].old_year  = sYear;
  buff[rtcDataLen].old_month = sMonth;
  buff[rtcDataLen].old_date  = sDate;
  buff[rtcDataLen].old_hour  = sHour;
  buff[rtcDataLen].old_minute= sMinute;
  buff[rtcDataLen].old_second= sSecond;

  buff[rtcDataLen].type      = sType;
  buff[rtcDataLen].period    = sPeriod;
  buff[rtcDataLen].count     = rtcDataLen;
  buff[rtcDataLen].offset    = sOffset;
  buff[rtcDataLen].number    = sNumber;
  memcpy(buff[rtcDataLen].arr ,sWeek ,sWeekLen);
  rtcDataLen++;
  BubbleSort();
  // flush_Alarm();
  RTC_Set_Alam(buff[0].year ,buff[0].month ,buff[0].date ,buff[0].hour ,buff[0].minute ,buff[0].second);
  
  if(0 == FlashWriteFlag) {   // 任务列表改变，写入flash
    FlashWriteFlag = 1;
  }
  AT_OK;
}

/**
 * @brief 对任务列表 按时间 进行 冒泡排序 
 * 
 */
void BubbleSort(void)
{

  int i, j;
  int flags = 0;
  struct uart_RTC_Time temp = {0};
  for (j = 0; j < rtcDataLen - 1; j++)
  {
    for (i = 0; i < rtcDataLen - 1 - j; i++)
    {
      if (buff[i].timePoint > buff[i + 1].timePoint)
      {
        memcpy(&temp      ,&buff[i]   ,sizeof(temp));
        memcpy(&buff[i]   ,&buff[i+1] ,sizeof(temp));
        memcpy(&buff[i+1] ,&temp      ,sizeof(temp));
        flags = 1;//不是有序的，flags设置????1????
      }
    }
    if (flags == 0)
      return;
  }
}

/**
 * @brief 在usb-cdc的接口打印帮助信息
 * 
 */
void HelpInformation(void)
{
  USB_SEND_DATA("\n|-----------------------------------------------------------|\n");
  USB_SEND_DATA("|-----------------------------------------------------------|\n");
  USB_SEND_DATA("| Basic commands                                            |\n");
  USB_SEND_DATA("|-----------------------------------------------------------|\n");
  USB_SEND_DATA("|-----------------------------------------------------------|\n");
  USB_SEND_DATA("| AT_HELP<CR>                                               |Get AT command information\n");
  USB_SEND_DATA("| AT_GET_TIME<CR>                                           |Get current time\n");
  USB_SEND_DATA("| AT_GET_ELECTRIC<CR>                                       |Get electric quantity information(Percentage and voltage)\n");
  USB_SEND_DATA("| AT_GET_PLAN_LIST<CR>                                      |Get task list\n");
  USB_SEND_DATA("|-----------------------------------------------------------|\n");
  USB_SEND_DATA("| AT_CLEAR_LIST_ALL<CR>                                     |Clear all task lists\n");
  USB_SEND_DATA("| AT_CLEAR_LIST=?<CR>                                       |Clear the list of which plan range: (1<= ? <=20)\n");
  USB_SEND_DATA("|-----                                                      |\n");
  USB_SEND_DATA("| e.g., AT_CLEAR_LIST=15<CR>                                |Clear the 15th task\n");
  USB_SEND_DATA("|-----------------------------------------------------------|\n");
  USB_SEND_DATA("| (2000 < year < 2100) (1 < month  < 12) (1 < date   < 31)  |\n");
  USB_SEND_DATA("| (0 < hour < 23)      (0 < minute < 59) (0 < second < 59)  |\n");
  USB_SEND_DATA("|-----------------------------------------------------------|\n");
  USB_SEND_DATA("| AT_SET_TIME=year/month/date/hour:minute:second<CR>        |Set time\n");
  USB_SEND_DATA("|-----                                                      |\n");
  USB_SEND_DATA("| e.g., AT_SET_TIME=2000/01/01/0:0:1<CR>                    |Set current time to 2020/01/01/00:00:01\n");
  USB_SEND_DATA("|-----------------------------------------------------------|\n");
  USB_SEND_DATA("| AT_ALARM_STARTING_UP=hour:minute:second<CR>               |Add a one-time alarm clock task for boot-up\n");
  USB_SEND_DATA("| AT_ALARM_SHUTDOWN=hour:minute:second<CR>                  |Add a one-time alarm clock task for shutdown\n");
  USB_SEND_DATA("| AT_ALARM_POWER_OFF=hour:minute:second<CR>                 |Add a one-time alarm clock task for power-off\n");
  USB_SEND_DATA("| AT_STARTING_UP=year/month/date/hour:minute:second<CR>     |Add one-time task for boot-up\n");
  USB_SEND_DATA("| AT_SHUTDOWN=year/month/date/hour:minute:second<CR>        |Add one-time task for shutdown\n");
  USB_SEND_DATA("| AT_POWER_OFF=year/month/date/hour:minute:second<CR>       |Add one-time task for power-off command\n");
  USB_SEND_DATA("|-----------------------------------------------------------|\n");
  USB_SEND_DATA("|-----------------------------------------------------------|\n");
  USB_SEND_DATA("| Advanced commands                                         |\n");
  USB_SEND_DATA("|-----------------------------------------------------------|\n");
  USB_SEND_DATA("|-----------------------------------------------------------|\n");
  USB_SEND_DATA("| 1=Monday ; 2=Tuesday ; 3=Wednesday ; 4=Thursday ;         |\n");
  USB_SEND_DATA("| 5=Friday ; 6=Saturday ; 7=Sunday                          |\n");
  USB_SEND_DATA("|-----------------------------------------------------------|\n");
  USB_SEND_DATA("| AT_ALARM_STARTING_UP=hour:minute:second/?<CR>             |Add a cyclical boot-up task for days of the week (?=1234567)\n");
  USB_SEND_DATA("| AT_ALARM_SHUTDOWN=hour:minute:second/?<CR>                |Add a cyclical shutdown task for days of the week (?=1234567)\n");
  USB_SEND_DATA("| AT_ALARM_POWER_OFF=hour:minute:second/?<CR>               |Add a cyclical power-off command task for days of the week (?=1234567)\n");
  USB_SEND_DATA("|-----                                                      |\n");
  USB_SEND_DATA("| e.g., AT_ALARM_STARTING_UP=XX:XX:XX/1356<CR>              |Add a boot-up alarm closk task for every Monday, Wednesday, Friday, Saturday\n");
  USB_SEND_DATA("|-----------------------------------------------------------|\n");
  USB_SEND_DATA("| Y=Year ; M=Month ; 2W=Two weeks ; W=Week ; D=Day          |\n");
  USB_SEND_DATA("|-----------------------------------------------------------|\n");
  USB_SEND_DATA("| AT_STARTING_UP=year/month/date/hour:minute:second/?<CR>   |Add a cyclical boot-up task for days of the year (?=Y/M/2W/W/D)\n");
  USB_SEND_DATA("| AT_SHUTDOWN=year/month/date/hour:minute:second/?<CR>      |Add a cyclical shutdown task for days of the year (?=Y/M/2W/W/D)\n");
  USB_SEND_DATA("| AT_POWER_OFF=year/month/date/hour:minute:second/?<CR>     |Add a cyclical power-off command task for days of the year (?=Y/M/2W/W/D)\n");
  USB_SEND_DATA("|-----------------------------------------------------------|\n");
  USB_SEND_DATA("|-----                                                      |\n");
  USB_SEND_DATA("| e.g., AT_STARTING_UP=XXXX/XX/XX/XX:XX:XX/M<CR>            |Add a cyclical boot-up task for every month\n");
  USB_SEND_DATA("|-----------------------------------------------------------|\n");
}

/**
 * @brief 字符串拆解
 * 
 * @param data 数据
 * @param recv 接收的数据
 * @param offset 偏移
 * @param recvLen 接收的长度
 * @param lastData 最后一个数据
 * @param len 数据长度
 * @param strLen 字符串长度
 * @param lastString 最后一个字符
 * @param compare1 左边界 从小到大
 * @param compare2 右边界
 * @param string1 第一个字符
 * @param string2 最后一个字符
 * @return uint8_t 字符串拆解结果，拆解成功返回1
 */
uint8_t analysisString(uint8_t *data , uint8_t *recv ,uint8_t *offset , uint8_t *recvLen ,uint16_t *lastData ,uint8_t len ,uint8_t strLen ,char *lastString ,uint16_t compare1 ,uint16_t compare2 ,char *string1 ,char *string2)
{

  for(uint8_t i = (*offset) ,j=0; i < len; i++,j++)
  {
    if(data[i] == lastString[0]){
      recv[j] = '\r';
      (*offset) = i+1;
      break;
    }else{
          // printf("strLen = %u\n ", strLen);
          // printf("len = %u\n ", len);
          // printf("*offset = %u\n ", *offset);
          // printf("j = %u\n ", j);
      if(j < strLen){
          // printf("data[i] = %u\n ", data[i]);
        if(data[i] >= string1[0] && data[i] <= string2[0]){
          recv[j] = data[i];
          (*recvLen)++;
        }else{
          //printf("data is not 0 - 9\n ");
          return 0;       // 数据错误 return
        }
      }else{
        //printf("data is excess len \n");
        return 0;       // 数据错误 return
      }
    }
  }
          // printf("*recvLen = %u\n ", *recvLen);
          // printf("*************************\n ");

  if(string2[0] == 'z')    // 字符串拆??? 不转字符直接返回
  {
    if(strncmp((const char *)"2W" ,(const char *)recv ,2) == 0){
      (*lastData) = PERIOD_TWO_WEEK;
    }else{
      switch(recv[0])
      {
        case 'Y':
          (*lastData) = PERIOD_ONE_YEAR;
          break;
        case 'M':
          (*lastData) = PERIOD_ONE_MONTH;
          break;
        case 'D':
          (*lastData) = PERIOD_ONE_DATE;
          break;
        case 'W':
          (*lastData) = PERIOD_ONE_WEEK;
          break;
        default:
          return 0;
          // break;
      }
    }
    return 1;
  }else if(string2[0] == '7'){
    if(0 == *recvLen)
    {
      //printf("recvLen = 0 \n");
      return 0;       // 数据错误 return
    }
    // 冒泡升序
    for(uint8_t i=0; i<(*recvLen)-1; i++){
      for(uint8_t j=0; j<(*recvLen)-1-i; j++){
        if(recv[j]>recv[j+1])
        {
          uint8_t temp   = recv[j];
          recv[j]     = recv[j+1];
          recv[j+1]   = temp;
        }
      }
    }

        uint8_t b[8] = {0};
    uint8_t index = 1;
    b[0] = recv[0] - 0x30;
    // 去重
    for(uint8_t i = 1; i < (*recvLen); i++)
    {
      if(recv[i] != recv[i-1])
      {
        b[index++] = recv[i] - 0x30;      //如果当前遍历的元素和前一个元素不相等
      }
    }
    memset(recv ,0 ,8);
    memcpy(recv ,b ,8);
    (*recvLen) = index;
    (*lastData) = PERIOD_ALARM;
    return 1;
  }


  (*lastData) = atoi((const char*)recv);
  if((*lastData) < compare1 || (*lastData) > compare2)
  {
    //printf("compare error \n");
    return 0;       // 数据错误 return
  }

  if((*recvLen) == 0)
  {
    //printf("recv  len is 0 \n");
    return 0;
  }
  return 1;
}

/**
 * @brief 解析AT指令并做出相应操作
 * 
 * @param data 收到的AT指令
 * @param len 指令的长度
 */
void Data_Analysis(uint8_t *data ,uint8_t len)
{
  uartLock = 1;
  uint8_t tYear[5]   = {0};
  uint8_t tMonth[3]  = {0};
  uint8_t tDate[3]   = {0};
  uint8_t tHour[3]   = {0};
  uint8_t tMinute[3] = {0};
  uint8_t tSecond[3] = {0};
  uint8_t tPeriod[8] = {0};
  uint8_t tDelete[3] = {0};

  uint8_t yearLen    = 0;
  uint8_t monthLen   = 0;
  uint8_t dateLen    = 0;
  uint8_t hourLen    = 0;
  uint8_t minuteLen  = 0;
  uint8_t secondLen  = 0;
  uint8_t periodLen  = 0;
  uint8_t deleteLen  = 0;

  uint16_t  yearData   = 0;
  uint16_t  monthData  = 0;
  uint16_t  dateData   = 0;
  uint16_t  hourData   = 0;
  uint16_t  minuteData = 0;
  uint16_t  secondData = 0;
  uint16_t  periodData = 0;
  uint16_t  deleteData = 0;

  uint8_t tCount1    = 0;              // 记录 / 出现的次
  uint8_t tCount2    = 0;              // 记录 : 出现的次
  uint8_t tCount3    = 0;              // 记录 = 出现的位
  uint8_t tCount4    = 0;              // 记录 = 出现的次

  //const char * xieGang = "/";
  //const char * maoHao  = ":";

  _nowTime time = RTC_Get();
#if 0
  printf("time.tm_oldTimePoint = %u\n", time.tm_oldTimePoint);
  printf("time.tm_year = %u\n", time.tm_year);
  printf("time.tm_month = %u\n", time.tm_month);
  printf("time.tm_date = %u\n", time.tm_date);
  printf("time.tm_week = %u\n", time.tm_week);
  printf("time.tm_hour = %u\n", time.tm_hour);
  printf("time.tm_minute = %u\n", time.tm_minute);
  printf("time.tm_second = %u\n", time.tm_second);
#endif

  uint32_t oldTimePoint = getTimePoint(time.tm_year, time.tm_month, time.tm_date, time.tm_hour, time.tm_minute, time.tm_second);
  uint8_t tweek = time.tm_week;

  for(uint8_t i = 0; i < len; i++)
  {
    if(data[i] == (uint8_t)'/')
      tCount1++;
    if(data[i] == (uint8_t)':')
      tCount2++;
    if(data[i] == (uint8_t)'=')
    {
      tCount3 = i + 1;
      tCount4++;
    }
  }

  // do something
  if(strncmp((const char *)"AT_SHUTDOWN=" ,(const char *)data ,strlen("AT_SHUTDOWN=")) == 0){
    if(tCount1 == 3 && tCount2 == 2 && tCount4 == 1){
      // 定时任务
      if(rtcDataLen >= MAX_LIST_LEN)
      {
        uartLock = 0;
        AT_LIST_FULL_ERROR;
        return;
      }

      if(0 == analysisString(data ,tYear ,&tCount3 ,&yearLen ,&yearData ,len ,4 ,"/" ,2000 ,2100 ,"0" ,"9"))
      {
        uartLock = 0;
        AT_DATA_ERROR;
        return;
      }

      if(0 == analysisString(data ,tMonth ,&tCount3 ,&monthLen ,&monthData ,len ,2 ,"/" ,1 ,12 ,"0" ,"9"))
      {
        uartLock = 0;
        AT_DATA_ERROR;
        return;
      }

      if(0 == analysisString(data ,tDate ,&tCount3 ,&dateLen ,&dateData ,len ,2 ,"/" ,1 ,31 ,"0" ,"9"))
      {
        uartLock = 0;
        AT_DATA_ERROR;
        return;
      }

      if(0 == analysisString(data ,tHour ,&tCount3 ,&hourLen ,&hourData ,len ,2 ,":" ,0 ,23 ,"0" ,"9"))
      {
        uartLock = 0;
        AT_DATA_ERROR;
        return;
      }

      if(0 == analysisString(data ,tMinute ,&tCount3 ,&minuteLen ,&minuteData ,len ,2 ,":" ,0 ,59 ,"0" ,"9"))
      {
        uartLock = 0;
        AT_DATA_ERROR;
        return;
      }

      if(0 == analysisString(data ,tSecond ,&tCount3 ,&secondLen ,&secondData ,len ,2 ,"\r" ,0 ,59 ,"0" ,"9"))
      {
        uartLock = 0;
        AT_DATA_ERROR;
        return;
      }

      if(1 != Check_date(yearData ,monthData ,dateData))
      {
        uartLock = 0;
        AT_DATA_ERROR;
        return;
      }
      uint32_t timePoint = getTimePoint(yearData,monthData,dateData,hourData,minuteData,secondData);

      if(oldTimePoint > timePoint)            // 单次 设置时间点不???????
      {
        uartLock = 0;
        AT_TIM_ERROR;
        return;
      }
      Assignment(timePoint ,yearData ,monthData ,dateData ,hourData ,minuteData ,secondData ,AT_SHUTDOWN ,PERIOD_ONCE ,0 ,0 ,tPeriod ,0);

    }else if(tCount1 == 4 && tCount2 == 2 && tCount4 == 1){
      // 循环任务
      if(rtcDataLen >= MAX_LIST_LEN)
      {
        uartLock = 0;
        AT_LIST_FULL_ERROR;
        return;
      }

      if(0 == analysisString(data ,tYear ,&tCount3 ,&yearLen ,&yearData ,len ,4 ,"/" ,2000 ,2100 ,"0" ,"9"))
      {
        uartLock = 0;
        AT_DATA_ERROR;
        return;
      }

      if(0 == analysisString(data ,tMonth ,&tCount3 ,&monthLen ,&monthData ,len ,2 ,"/" ,1 ,12 ,"0" ,"9"))
      {
        uartLock = 0;
        AT_DATA_ERROR;
        return;
      }

      if(0 == analysisString(data ,tDate ,&tCount3 ,&dateLen ,&dateData ,len ,2 ,"/" ,1 ,31 ,"0" ,"9"))
      {
        uartLock = 0;
        AT_DATA_ERROR;
        return;
      }

      if(0 == analysisString(data ,tHour ,&tCount3 ,&hourLen ,&hourData ,len ,2 ,":" ,0 ,23 ,"0" ,"9"))
      {
        uartLock = 0;
        AT_DATA_ERROR;
        return;
      }

      if(0 == analysisString(data ,tMinute ,&tCount3 ,&minuteLen ,&minuteData ,len ,2 ,":" ,0 ,59 ,"0" ,"9"))
      {
        uartLock = 0;
        AT_DATA_ERROR;
        return;
      }

      if(0 == analysisString(data ,tSecond ,&tCount3 ,&secondLen ,&secondData ,len ,2 ,"/" ,0 ,59 ,"0" ,"9"))
      {
        uartLock = 0;
        AT_DATA_ERROR;
        return;
      }


      if(0 == analysisString(data ,tPeriod ,&tCount3 ,&periodLen ,&periodData ,len ,2 ,"\r" ,0 ,0 ,"0" ,"z"))
      {
        uartLock = 0;
        AT_DATA_ERROR;
        return;
      }

      if(1 != Check_date(yearData ,monthData ,dateData))
      {
        uartLock = 0;
        AT_DATA_ERROR;
        return;
      }
      uint32_t timePoint = getTimePoint(yearData,monthData,dateData,hourData,minuteData,secondData);

      if(oldTimePoint > timePoint)            // 单次 设置时间点不对，直接
      {
        uartLock = 0;
        AT_TIM_ERROR;
        return;
      }
      Assignment(timePoint ,yearData ,monthData ,dateData ,hourData ,minuteData ,secondData ,AT_SHUTDOWN ,periodData ,0 ,0 ,tPeriod ,0);

    }else{
      // 数据错误
      AT_DATA_ERROR;
    }
  }else if(strncmp((const char *)"AT_POWER_OFF=" ,(const char *)data ,strlen("AT_POWER_OFF")) == 0){
    if(tCount1 == 3 && tCount2 == 2 && tCount4 == 1){
      // 定时任务
      if(rtcDataLen >= MAX_LIST_LEN)
      {
        uartLock = 0;
        AT_LIST_FULL_ERROR;
        return;
      }
      if(0 == analysisString(data ,tYear ,&tCount3 ,&yearLen ,&yearData ,len ,4 ,"/" ,2000 ,2100 ,"0" ,"9"))
      {
        uartLock = 0;
        AT_DATA_ERROR;
        return;
      }

      if(0 == analysisString(data ,tMonth ,&tCount3 ,&monthLen ,&monthData ,len ,2 ,"/" ,1 ,12 ,"0" ,"9"))
      {
        uartLock = 0;
        AT_DATA_ERROR;
        return;
      }

      if(0 == analysisString(data ,tDate ,&tCount3 ,&dateLen ,&dateData ,len ,2 ,"/" ,1 ,31 ,"0" ,"9"))
      {
        uartLock = 0;
        AT_DATA_ERROR;
        return;
      }

      if(0 == analysisString(data ,tHour ,&tCount3 ,&hourLen ,&hourData ,len ,2 ,":" ,0 ,23 ,"0" ,"9"))
      {
        uartLock = 0;
        AT_DATA_ERROR;
        return;
      }

      if(0 == analysisString(data ,tMinute ,&tCount3 ,&minuteLen ,&minuteData ,len ,2 ,":" ,0 ,59 ,"0" ,"9"))
      {
        uartLock = 0;
        AT_DATA_ERROR;
        return;
      }

      if(0 == analysisString(data ,tSecond ,&tCount3 ,&secondLen ,&secondData ,len ,2 ,"\r" ,0 ,59 ,"0" ,"9"))
      {
        uartLock = 0;
        AT_DATA_ERROR;
        return;
      }

      if(1 != Check_date(yearData ,monthData ,dateData))
      {
        uartLock = 0;
        AT_DATA_ERROR;
        return;
      }

      uint32_t timePoint = getTimePoint(yearData,monthData,dateData,hourData,minuteData,secondData);
      if(oldTimePoint > timePoint)            // 单次 设置时间点不???????
      {
        uartLock = 0;
        AT_TIM_ERROR;
        return;
      }

      Assignment(timePoint ,yearData ,monthData ,dateData ,hourData ,minuteData ,secondData ,AT_POWER_OFF ,PERIOD_ONCE ,0 ,0 ,tPeriod ,0);

    }else if(tCount1 == 4 && tCount2 == 2 && tCount4 == 1){
      // 循环任务
      if(rtcDataLen >= MAX_LIST_LEN)
      {
        uartLock = 0;
        AT_LIST_FULL_ERROR;
        return;
      }

      if(0 == analysisString(data ,tYear ,&tCount3 ,&yearLen ,&yearData ,len ,4 ,"/" ,2000 ,2100 ,"0" ,"9"))
      {
        uartLock = 0;
        AT_DATA_ERROR;
        return;
      }

      if(0 == analysisString(data ,tMonth ,&tCount3 ,&monthLen ,&monthData ,len ,2 ,"/" ,1 ,12 ,"0" ,"9"))
      {
        uartLock = 0;
        AT_DATA_ERROR;
        return;
      }

      if(0 == analysisString(data ,tDate ,&tCount3 ,&dateLen ,&dateData ,len ,2 ,"/" ,1 ,31 ,"0" ,"9"))
      {
        uartLock = 0;
        AT_DATA_ERROR;
        return;
      }

      if(0 == analysisString(data ,tHour ,&tCount3 ,&hourLen ,&hourData ,len ,2 ,":" ,0 ,23 ,"0" ,"9"))
      {
        uartLock = 0;
        AT_DATA_ERROR;
        return;
      }

      if(0 == analysisString(data ,tMinute ,&tCount3 ,&minuteLen ,&minuteData ,len ,2 ,":" ,0 ,59 ,"0" ,"9"))
      {
        uartLock = 0;
        AT_DATA_ERROR;
        return;
      }

      if(0 == analysisString(data ,tSecond ,&tCount3 ,&secondLen ,&secondData ,len ,2 ,"/" ,0 ,59 ,"0" ,"9"))
      {
        uartLock = 0;
        AT_DATA_ERROR;
        return;
      }


      if(0 == analysisString(data ,tPeriod ,&tCount3 ,&periodLen ,&periodData ,len ,2 ,"\r" ,0 ,0 ,"0" ,"z"))
      {
        uartLock = 0;
        AT_DATA_ERROR;
        return;
      }

      if(1 != Check_date(yearData ,monthData ,dateData))
      {
        uartLock = 0;
        AT_DATA_ERROR;
        return;
      }
      //printf("%d/%d/%d/%d:%d:%d\n" ,yearData ,monthData ,dateData ,hourData ,minuteData ,secondData);
      uint32_t timePoint = getTimePoint(yearData,monthData,dateData,hourData,minuteData,secondData);


      if(oldTimePoint > timePoint)            // 单次 设置时间点不???????
      {
        uartLock = 0;
        AT_TIM_ERROR;
        return;
      }

      Assignment(timePoint ,yearData ,monthData ,dateData ,hourData ,minuteData ,secondData ,AT_POWER_OFF ,periodData ,0 ,0 ,tPeriod ,0);

    }else{
      // 数据错误
      AT_DATA_ERROR;

    }
  }else if(strncmp((const char *)"AT_STARTING_UP=" ,(const char *)data ,strlen("AT_STARTING_UP=")) == 0){
    if(tCount1 == 3 && tCount2 == 2 && tCount4 == 1){
      // 定时任务
      if(rtcDataLen >= MAX_LIST_LEN)
      {
        uartLock = 0;
        AT_LIST_FULL_ERROR;
        return;
      }
      if(0 == analysisString(data ,tYear ,&tCount3 ,&yearLen ,&yearData ,len ,4 ,"/" ,2000 ,2100 ,"0" ,"9"))
      {
        uartLock = 0;
        AT_DATA_ERROR;
        return;
      }

      if(0 == analysisString(data ,tMonth ,&tCount3 ,&monthLen ,&monthData ,len ,2 ,"/" ,1 ,12 ,"0" ,"9"))
      {
        uartLock = 0;
        AT_DATA_ERROR;
        return;
      }

      if(0 == analysisString(data ,tDate ,&tCount3 ,&dateLen ,&dateData ,len ,2 ,"/" ,1 ,31 ,"0" ,"9"))
      {
        uartLock = 0;
        AT_DATA_ERROR;
        return;
      }

      if(0 == analysisString(data ,tHour ,&tCount3 ,&hourLen ,&hourData ,len ,2 ,":" ,0 ,23 ,"0" ,"9"))
      {
        uartLock = 0;
        AT_DATA_ERROR;
        return;
      }

      if(0 == analysisString(data ,tMinute ,&tCount3 ,&minuteLen ,&minuteData ,len ,2 ,":" ,0 ,59 ,"0" ,"9"))
      {
        uartLock = 0;
        AT_DATA_ERROR;
        return;
      }

      if(0 == analysisString(data ,tSecond ,&tCount3 ,&secondLen ,&secondData ,len ,2 ,"\r" ,0 ,59 ,"0" ,"9"))
      {
        uartLock = 0;
        AT_DATA_ERROR;
        return;
      }

      if(1 != Check_date(yearData ,monthData ,dateData))
      {
        uartLock = 0;
        AT_DATA_ERROR;
        return;
      }

      uint32_t timePoint = getTimePoint(yearData,monthData,dateData,hourData,minuteData,secondData);
      if(oldTimePoint > timePoint)            // 单次 设置时间点不???????
      {
        uartLock = 0;
        AT_TIM_ERROR;
        return;
      }

      Assignment(timePoint ,yearData ,monthData ,dateData ,hourData ,minuteData ,secondData ,AT_START_UP ,PERIOD_ONCE ,0 ,0 ,tPeriod ,0);

    }else if(tCount1 == 4 && tCount2 == 2 && tCount4 == 1){
      // 循环任务
      if(rtcDataLen >= MAX_LIST_LEN)
      {
        uartLock = 0;
        AT_LIST_FULL_ERROR;
        return;
      }

      if(0 == analysisString(data ,tYear ,&tCount3 ,&yearLen ,&yearData ,len ,4 ,"/" ,2000 ,2100 ,"0" ,"9"))
      {
        uartLock = 0;
        AT_DATA_ERROR;
        return;
      }

      if(0 == analysisString(data ,tMonth ,&tCount3 ,&monthLen ,&monthData ,len ,2 ,"/" ,1 ,12 ,"0" ,"9"))
      {
        uartLock = 0;
        AT_DATA_ERROR;
        return;
      }

      if(0 == analysisString(data ,tDate ,&tCount3 ,&dateLen ,&dateData ,len ,2 ,"/" ,1 ,31 ,"0" ,"9"))
      {
        uartLock = 0;
        AT_DATA_ERROR;
        return;
      }

      if(0 == analysisString(data ,tHour ,&tCount3 ,&hourLen ,&hourData ,len ,2 ,":" ,0 ,23 ,"0" ,"9"))
      {
        uartLock = 0;
        AT_DATA_ERROR;
        return;
      }

      if(0 == analysisString(data ,tMinute ,&tCount3 ,&minuteLen ,&minuteData ,len ,2 ,":" ,0 ,59 ,"0" ,"9"))
      {
        uartLock = 0;
        AT_DATA_ERROR;
        return;
      }

      if(0 == analysisString(data ,tSecond ,&tCount3 ,&secondLen ,&secondData ,len ,2 ,"/" ,0 ,59 ,"0" ,"9"))
      {
        uartLock = 0;
        AT_DATA_ERROR;
        return;
      }


      if(0 == analysisString(data ,tPeriod ,&tCount3 ,&periodLen ,&periodData ,len ,2 ,"\r" ,0 ,0 ,"0" ,"z"))
      {
        uartLock = 0;
        AT_DATA_ERROR;
        return;
      }

      if(1 != Check_date(yearData ,monthData ,dateData))
      {
        uartLock = 0;
        AT_DATA_ERROR;
        return;
      }
      //printf("%d/%d/%d/%d:%d:%d\n" ,yearData ,monthData ,dateData ,hourData ,minuteData ,secondData);
      uint32_t timePoint = getTimePoint(yearData,monthData,dateData,hourData,minuteData,secondData);


      if(oldTimePoint > timePoint)            // 单次 设置时间点不???????
      {
        uartLock = 0;
        AT_TIM_ERROR;
        return;
      }

      Assignment(timePoint ,yearData ,monthData ,dateData ,hourData ,minuteData ,secondData ,AT_START_UP ,periodData ,0 ,0 ,tPeriod ,0);

    }else{
      // 数据错误
      AT_DATA_ERROR;

    }
  }else if(strncmp("AT_ALARM_POWER_OFF=" ,(const char *)data ,strlen("AT_ALARM_POWER_OFF=")) == 0){
    if(tCount1 == 0 && tCount2 == 2 && tCount4 == 1){
      // 没有参数不循???????
      if(rtcDataLen >= MAX_LIST_LEN)
      {
        uartLock = 0;
        AT_LIST_FULL_ERROR;
        return;
      }

      if(0 == analysisString(data ,tHour ,&tCount3 ,&hourLen ,&hourData ,len ,2 ,":" ,0 ,23 ,"0" ,"9"))
      {
        uartLock = 0;
        AT_DATA_ERROR;
        return;
      }

      if(0 == analysisString(data ,tMinute ,&tCount3 ,&minuteLen ,&minuteData ,len ,2 ,":" ,0 ,59 ,"0" ,"9"))
      {
        uartLock = 0;
        AT_DATA_ERROR;
        return;
      }

      if(0 == analysisString(data ,tSecond ,&tCount3 ,&secondLen ,&secondData ,len ,2 ,"\r" ,0 ,59 ,"0" ,"9"))
      {
        uartLock = 0;
        AT_DATA_ERROR;
        return;
      }
      yearData  = time.tm_year;
      monthData = time.tm_month;
      dateData  = time.tm_date;

      if(hourData > time.tm_hour){
        // 当天
      }else if(hourData == time.tm_hour){
        if(minuteData > time.tm_minute){
          // 当天
        }else if(minuteData == time.tm_minute){
          if(secondData > time.tm_second){
            // 当天
          }else{
            //第二???
            add_date(&yearData ,&monthData ,&dateData ,1);
          }
        }else{
          // 第二???
          add_date(&yearData ,&monthData ,&dateData ,1);
        }
      }else{
        // 第二???
        add_date(&yearData ,&monthData ,&dateData ,1);
      }

      uint32_t timePoint = getTimePoint(yearData,monthData,dateData,hourData,minuteData,secondData);

      Assignment(timePoint ,yearData ,monthData ,dateData ,hourData ,minuteData ,secondData ,AT_ALARM_POWER_OFF ,PERIOD_ALARM_ONCE ,0 ,0 ,tPeriod ,0);

    }else if(tCount1 == 1 && tCount2 == 2 && tCount4 == 1){
      // 循环
      if(rtcDataLen >= MAX_LIST_LEN)
      {
        uartLock = 0;
        AT_LIST_FULL_ERROR;
        return;
      }
      if(0 == analysisString(data ,tHour ,&tCount3 ,&hourLen ,&hourData ,len ,2 ,":" ,0 ,23 ,"0" ,"9"))
      {
        uartLock = 0;
        AT_DATA_ERROR;
        return;
      }

      if(0 == analysisString(data ,tMinute ,&tCount3 ,&minuteLen ,&minuteData ,len ,2 ,":" ,0 ,59 ,"0" ,"9"))
      {
        uartLock = 0;
        AT_DATA_ERROR;
        return;
      }

      if(0 == analysisString(data ,tSecond ,&tCount3 ,&secondLen ,&secondData ,len ,2 ,"/" ,0 ,59 ,"0" ,"9"))
      {
        uartLock = 0;
        AT_DATA_ERROR;
        return;
      }

      if(0 == analysisString(data ,tPeriod ,&tCount3 ,&periodLen ,&periodData ,len ,7 ,"\r" ,0 ,59 ,"1" ,"7"))
      {
        uartLock = 0;
        AT_DATA_ERROR;
        return;
      }
      yearData  = time.tm_year;
      monthData = time.tm_month;
      dateData  = time.tm_date;
      uint8_t periodCount =  0;
      uint8_t ii          =  0;
      for(ii = 0; ii < periodLen; ii++)
      {
        if(tPeriod[ii] > tweek){
          periodCount = 1;                  //设置星期大于等于当前星期
          break;
        }else if(tPeriod[ii] == tweek){
          periodCount = 2;
          break;
        }else{
          periodCount = 0;
        }
      }

      switch(periodCount)
      {
        case 0:
          // 设置星期小于当前星期
          ii = 0;
          add_date(&yearData ,&monthData ,&dateData ,7 - tweek + tPeriod[0]);
          break;
        case 1:
          add_date(&yearData ,&monthData ,&dateData ,tPeriod[ii] - tweek);
          break;
        case 2:
          if(hourData > time.tm_hour){
            // 当天
          }else if(hourData == time.tm_hour){
            if(minuteData > time.tm_minute){
              // 当天
            }else if(minuteData == time.tm_minute){
              if(secondData > time.tm_second){
                // 当天
              }else{
                if(ii + 1 >= periodLen)
                {
                  ii = 0;
                  add_date(&yearData ,&monthData ,&dateData ,7 - tweek + tPeriod[ii]);
                }else{
                  ii++;
                  add_date(&yearData ,&monthData ,&dateData ,tPeriod[ii] - tweek);
                }
              }
            }else{
              if(ii + 1 >= periodLen)
              {
                ii = 0;
                add_date(&yearData ,&monthData ,&dateData ,7 - tweek + tPeriod[ii]);
              }else{
                ii++;
                add_date(&yearData ,&monthData ,&dateData ,tPeriod[ii] - tweek);
              }
            }
          }else{
            if(ii + 1 >= periodLen)
            {
              ii = 0;
              add_date(&yearData ,&monthData ,&dateData ,7 - tweek + tPeriod[ii]);
            }else{
              ii++;
              add_date(&yearData ,&monthData ,&dateData ,tPeriod[ii] - tweek);
            }
          }
          break;
        default:
          break;
      }
      uint32_t timePoint = getTimePoint(yearData,monthData,dateData,hourData,minuteData,secondData);

      Assignment(timePoint ,yearData ,monthData ,dateData ,hourData ,minuteData ,secondData ,AT_ALARM_POWER_OFF ,PERIOD_ALARM ,ii ,periodLen ,tPeriod ,periodLen);

    }else{
      // 数据错误
      AT_DATA_ERROR;
    }
  }else if(strncmp("AT_ALARM_SHUTDOWN=" ,(const char *)data ,strlen("AT_ALARM_SHUTDOWN=")) == 0){
    if(tCount1 == 0 && tCount2 == 2 && tCount4 == 1){
      // 没有参数不循???????
      if(rtcDataLen >= MAX_LIST_LEN)
      {
        uartLock = 0;
        AT_LIST_FULL_ERROR;
        return;
      }

      if(0 == analysisString(data ,tHour ,&tCount3 ,&hourLen ,&hourData ,len ,2 ,":" ,0 ,23 ,"0" ,"9"))
      {
        uartLock = 0;
        AT_DATA_ERROR;
        return;
      }

      if(0 == analysisString(data ,tMinute ,&tCount3 ,&minuteLen ,&minuteData ,len ,2 ,":" ,0 ,59 ,"0" ,"9"))
      {
        uartLock = 0;
        AT_DATA_ERROR;
        return;
      }

      if(0 == analysisString(data ,tSecond ,&tCount3 ,&secondLen ,&secondData ,len ,2 ,"\r" ,0 ,59 ,"0" ,"9"))
      {
        uartLock = 0;
        AT_DATA_ERROR;
        return;
      }
      yearData  = time.tm_year;
      monthData = time.tm_month;
      dateData  = time.tm_date;

      if(hourData > time.tm_hour){
        // 当天
      }else if(hourData == time.tm_hour){
        if(minuteData > time.tm_minute){
          // 当天
        }else if(minuteData == time.tm_minute){
          if(secondData > time.tm_second){
            // 当天
          }else{
            //第二???
            add_date(&yearData ,&monthData ,&dateData ,1);
          }
        }else{
          // 第二???
          add_date(&yearData ,&monthData ,&dateData ,1);
        }
      }else{
        // 第二???
        add_date(&yearData ,&monthData ,&dateData ,1);
      }

      uint32_t timePoint = getTimePoint(yearData,monthData,dateData,hourData,minuteData,secondData);

      Assignment(timePoint ,yearData ,monthData ,dateData ,hourData ,minuteData ,secondData ,AT_ALARM_SHUTDOWN ,PERIOD_ALARM_ONCE ,0 ,0 ,tPeriod ,0);

    }else if(tCount1 == 1 && tCount2 == 2 && tCount4 == 1){
      // 循环
      if(rtcDataLen >= MAX_LIST_LEN)
      {
        uartLock = 0;
        AT_LIST_FULL_ERROR;
        return;
      }
      if(0 == analysisString(data ,tHour ,&tCount3 ,&hourLen ,&hourData ,len ,2 ,":" ,0 ,23 ,"0" ,"9"))
      {
        uartLock = 0;
        AT_DATA_ERROR;
        return;
      }

      if(0 == analysisString(data ,tMinute ,&tCount3 ,&minuteLen ,&minuteData ,len ,2 ,":" ,0 ,59 ,"0" ,"9"))
      {
        uartLock = 0;
        AT_DATA_ERROR;
        return;
      }

      if(0 == analysisString(data ,tSecond ,&tCount3 ,&secondLen ,&secondData ,len ,2 ,"/" ,0 ,59 ,"0" ,"9"))
      {
        uartLock = 0;
        AT_DATA_ERROR;
        return;
      }

      if(0 == analysisString(data ,tPeriod ,&tCount3 ,&periodLen ,&periodData ,len ,7 ,"\r" ,0 ,59 ,"1" ,"7"))
      {
        uartLock = 0;
        AT_DATA_ERROR;
        return;
      }
      yearData  = time.tm_year;
      monthData = time.tm_month;
      dateData  = time.tm_date;
      uint8_t periodCount =  0;
      uint8_t ii          =  0;
      for(ii = 0; ii < periodLen; ii++)
      {
        if(tPeriod[ii] > tweek){
          periodCount = 1;                  //设置星期大于等于当前星期
          break;
        }else if(tPeriod[ii] == tweek){
          periodCount = 2;
          break;
        }else{
          periodCount = 0;
        }
      }

      switch(periodCount)
      {
        case 0:
          // 设置星期小于当前星期
          ii = 0;
          add_date(&yearData ,&monthData ,&dateData ,7 - tweek + tPeriod[0]);
          break;
        case 1:
          add_date(&yearData ,&monthData ,&dateData ,tPeriod[ii] - tweek);
          break;
        case 2:
          if(hourData > time.tm_hour){
            // 当天
          }else if(hourData == time.tm_hour){
            if(minuteData > time.tm_minute){
              // 当天
            }else if(minuteData == time.tm_minute){
              if(secondData > time.tm_second){
                // 当天
              }else{
                if(ii + 1 >= periodLen)
                {
                  ii = 0;
                  add_date(&yearData ,&monthData ,&dateData ,7 - tweek + tPeriod[ii]);
                }else{
                  ii++;
                  add_date(&yearData ,&monthData ,&dateData ,tPeriod[ii] - tweek);
                }
              }
            }else{
              if(ii + 1 >= periodLen)
              {
                ii = 0;
                add_date(&yearData ,&monthData ,&dateData ,7 - tweek + tPeriod[ii]);
              }else{
                ii++;
                add_date(&yearData ,&monthData ,&dateData ,tPeriod[ii] - tweek);
              }
            }
          }else{
            if(ii + 1 >= periodLen)
            {
              ii = 0;
              add_date(&yearData ,&monthData ,&dateData ,7 - tweek + tPeriod[ii]);
            }else{
              ii++;
              add_date(&yearData ,&monthData ,&dateData ,tPeriod[ii] - tweek);
            }
          }
          break;
        default:
          break;
      }
      uint32_t timePoint = getTimePoint(yearData,monthData,dateData,hourData,minuteData,secondData);

      Assignment(timePoint ,yearData ,monthData ,dateData ,hourData ,minuteData ,secondData ,AT_ALARM_SHUTDOWN ,PERIOD_ALARM ,ii ,periodLen ,tPeriod ,periodLen);

    }else{
      // 数据错误
      AT_DATA_ERROR;
    }

  }else if(strncmp("AT_ALARM_STARTING_UP=" ,(const char *)data ,strlen("AT_ALARM_STARTING_UP=")) == 0){
    if(tCount1 == 0 && tCount2 == 2 && tCount4 == 1){
      // 没有参数不循???????
      if(rtcDataLen >= MAX_LIST_LEN)
      {
        uartLock = 0;
        AT_LIST_FULL_ERROR;
        return;
      }
      if(0 == analysisString(data ,tHour ,&tCount3 ,&hourLen ,&hourData ,len ,2 ,":" ,0 ,23 ,"0" ,"9"))
      {
        uartLock = 0;
        AT_DATA_ERROR;
        return;
      }

      if(0 == analysisString(data ,tMinute ,&tCount3 ,&minuteLen ,&minuteData ,len ,2 ,":" ,0 ,59 ,"0" ,"9"))
      {
        uartLock = 0;
        AT_DATA_ERROR;
        return;
      }

      if(0 == analysisString(data ,tSecond ,&tCount3 ,&secondLen ,&secondData ,len ,2 ,"\r" ,0 ,59 ,"0" ,"9"))
      {
        uartLock = 0;
        AT_DATA_ERROR;
        return;
      }
      yearData  = time.tm_year;
      monthData = time.tm_month;
      dateData  = time.tm_date;

      if(hourData > time.tm_hour){
        // 当天
      }else if(hourData == time.tm_hour){
        if(minuteData > time.tm_minute){
          // 当天
        }else if(minuteData == time.tm_minute){
          if(secondData > time.tm_second){
            // 当天
          }else{
            //第二???
            add_date(&yearData ,&monthData ,&dateData ,1);
          }
        }else{
          // 第二???
          add_date(&yearData ,&monthData ,&dateData ,1);
        }
      }else{
        // 第二???
        add_date(&yearData ,&monthData ,&dateData ,1);
      }

      uint32_t timePoint = getTimePoint(yearData,monthData,dateData,hourData,minuteData,secondData);
      Assignment(timePoint ,yearData ,monthData ,dateData ,hourData ,minuteData ,secondData ,AT_ALARM_START_UP ,PERIOD_ALARM_ONCE ,0 ,0 ,tPeriod ,0);

    }else if(tCount1 == 1 && tCount2 == 2 && tCount4 == 1){
      // 循环
      if(rtcDataLen >= MAX_LIST_LEN)
      {
        uartLock = 0;
        AT_LIST_FULL_ERROR;
        return;
      }

      if(0 == analysisString(data ,tHour ,&tCount3 ,&hourLen ,&hourData ,len ,2 ,":" ,0 ,23 ,"0" ,"9"))
      {
        uartLock = 0;
        AT_DATA_ERROR;
        return;
      }

      if(0 == analysisString(data ,tMinute ,&tCount3 ,&minuteLen ,&minuteData ,len ,2 ,":" ,0 ,59 ,"0" ,"9"))
      {
        uartLock = 0;
        AT_DATA_ERROR;
        return;
      }

      if(0 == analysisString(data ,tSecond ,&tCount3 ,&secondLen ,&secondData ,len ,2 ,"/" ,0 ,59 ,"0" ,"9"))
      {
        uartLock = 0;
        AT_DATA_ERROR;
        return;
      }

      if(0 == analysisString(data ,tPeriod ,&tCount3 ,&periodLen ,&periodData ,len ,7 ,"\r" ,0 ,59 ,"1" ,"7"))
      {
        uartLock = 0;
        AT_DATA_ERROR;
        return;
      }

      yearData  = time.tm_year;
      monthData = time.tm_month;
      dateData  = time.tm_date;

      uint8_t periodCount =  0;
      uint8_t ii          =  0;
      for(ii = 0; ii < periodLen; ii++)
      {
        if(tPeriod[ii] > tweek){
          periodCount = 1;                  //设置星期大于等于当前星期
          break;
        }else if(tPeriod[ii] == tweek){
          periodCount = 2;
          break;
        }else{
          periodCount = 0;
        }
      }

      // printf("periodCount = %u\n", periodCount);

      switch(periodCount)
      {
        case 0:
          // 设置星期小于当前星期
          ii = 0;
          add_date(&yearData ,&monthData ,&dateData ,7 - tweek + tPeriod[0]);
          break;
        case 1:
          add_date(&yearData ,&monthData ,&dateData ,tPeriod[ii] - tweek);
          break;
        case 2:
          if(hourData > time.tm_hour){
            // 当天
          }else if(hourData == time.tm_hour){
            if(minuteData > time.tm_minute){
              // 当天
            }else if(minuteData == time.tm_minute){
              if(secondData > time.tm_second){
                // 当天
              }else{
                if(ii + 1 >= periodLen)
                {
                  ii = 0;
                  add_date(&yearData ,&monthData ,&dateData ,7 - tweek + tPeriod[ii]);
                }else{
                  ii++;
                  add_date(&yearData ,&monthData ,&dateData ,tPeriod[ii] - tweek);
                }
              }
            }else{
              if(ii + 1 >= periodLen)
              {
                ii = 0;
                add_date(&yearData ,&monthData ,&dateData ,7 - tweek + tPeriod[ii]);
              }else{
                ii++;
                add_date(&yearData ,&monthData ,&dateData ,tPeriod[ii] - tweek);
              }
            }
          }else{
            if(ii + 1 >= periodLen)
            {
              ii = 0;
              add_date(&yearData ,&monthData ,&dateData ,7 - tweek + tPeriod[ii]);
            }else{
              ii++;
              add_date(&yearData ,&monthData ,&dateData ,tPeriod[ii] - tweek);
            }
          }
          break;
        default:
          break;
      }

      uint32_t timePoint = getTimePoint(yearData,monthData,dateData,hourData,minuteData,secondData);

      Assignment(timePoint ,yearData ,monthData ,dateData ,hourData ,minuteData ,secondData ,AT_ALARM_START_UP ,
      periodData ,ii ,periodLen ,tPeriod ,periodLen);

    }else{
      // 数据错误
      AT_DATA_ERROR;
    }

  }else if(strncmp("AT_SET_TIME=" ,(const char *)data ,strlen("AT_SET_TIME=")) == 0){

    if(0 == analysisString(data ,tYear ,&tCount3 ,&yearLen ,&yearData ,len ,4 ,"/" ,2000 ,2100 ,"0" ,"9"))
    {
      uartLock = 0;
      AT_DATA_ERROR;
      return;
    }

    if(0 == analysisString(data ,tMonth ,&tCount3 ,&monthLen ,&monthData ,len ,2 ,"/" ,1 ,12 ,"0" ,"9"))
    {
      uartLock = 0;
      AT_DATA_ERROR;
      return;
    }

    if(0 == analysisString(data ,tDate ,&tCount3 ,&dateLen ,&dateData ,len ,2 ,"/" ,1 ,31 ,"0" ,"9"))
    {
      uartLock = 0;
      AT_DATA_ERROR;
      return;
    }

    if(0 == analysisString(data ,tHour ,&tCount3 ,&hourLen ,&hourData ,len ,2 ,":" ,0 ,23 ,"0" ,"9"))
    {
      uartLock = 0;
      AT_DATA_ERROR;
      return;
    }

    if(0 == analysisString(data ,tMinute ,&tCount3 ,&minuteLen ,&minuteData ,len ,2 ,":" ,0 ,59 ,"0" ,"9"))
    {
      uartLock = 0;
      AT_DATA_ERROR;
      return;
    }

    if(0 == analysisString(data ,tSecond ,&tCount3 ,&secondLen ,&secondData ,len ,2 ,"\r" ,0 ,59 ,"0" ,"9"))
    {
      uartLock = 0;
      AT_DATA_ERROR;
      return;
    }
    uint32_t timePoint = getTimePoint(yearData,monthData,dateData,hourData,minuteData,secondData);
    if(1 != Check_date(yearData ,monthData ,dateData))
    {
      uartLock = 0;
      AT_DATA_ERROR;
      return;
    }
    
    uint16_t updateWeek = RTC_Get_Week(yearData,monthData,dateData);
    if(updateWeek == 0)  updateWeek = 7;
    printf("AT_SET_TIME=%d/%d/%d/%d:%d:%d\n",yearData,monthData,dateData,hourData,minuteData,secondData);
    printf("buff[0].timePoint = %u timePoint = %u\n",buff[0].timePoint, timePoint);
    if(buff[0].timePoint < timePoint){
      updateAlarm(updateWeek, 1,yearData,monthData,dateData,hourData,minuteData,secondData); //更新闹钟日期的刷新
      // RTC_Set_Alam(buff[0].year ,buff[0].month ,buff[0].date ,buff[0].hour ,buff[0].minute ,buff[0].second);

    }else{
  RTC_Set_Alam(buff[0].year ,buff[0].month ,buff[0].date ,buff[0].hour ,buff[0].minute ,buff[0].second);
        uint8_t periodCount =  0;
        uint8_t ii          =  0;
      switch(buff[0].period)
      {
        // uint8_t count = 0;
        case PERIOD_ALARM:
        buff[0].year   = yearData;
        buff[0].month  = monthData;
        buff[0].date   = dateData;

        for(ii = 0; ii < buff[0].number; ii++)
        {
          if(buff[0].arr[ii] > updateWeek){
            periodCount = 1;                  //设置星期大于等于当前星期
            //ii++;
            break;
          }else if(buff[0].arr[ii] == updateWeek){
            periodCount = 2;
            //ii++;
            break;
          }else{
            periodCount = 0;
          }
        }
        switch(periodCount)
        {
          case 0:
            // 设置星期小于当前星期
            ii = 0;
            //printf("xiao yu %d \n",ii);
            add_date(&buff[0].year ,&buff[0].month ,&buff[0].date ,7 - updateWeek + buff[0].arr[0]);
            break;
          case 1:
            //printf("da yu  %d \n",ii);
            add_date(&buff[0].year ,&buff[0].month ,&buff[0].date ,buff[0].arr[ii] - updateWeek);
            break;
          case 2:
            //printf("deng yu %d \n",ii);
            if(buff[0].hour > hourData){
              // 当天
            }else if(buff[0].hour == hourData){
              if(buff[0].minute > minuteData){
                // 当天
              }else if(buff[0].minute == minuteData){
                if(buff[0].second > secondData){
                  // 当天
                }else{
                  if(ii + 1 > buff[0].number)
                  {
                    ii = 0;
                    add_date(&buff[0].year ,&buff[0].month ,&buff[0].date ,7 - updateWeek + buff[0].arr[ii]);
                  }else{
                    ii++;
                    add_date(&buff[0].year ,&buff[0].month ,&buff[0].date ,buff[0].arr[ii] - updateWeek);
                  }
                }
              }else{
                if(ii + 1 > buff[0].number)
                {
                  ii = 0;
                  add_date(&buff[0].year ,&buff[0].month ,&buff[0].date ,7 - updateWeek + buff[0].arr[ii]);
                }else{
                  ii++;
                  add_date(&buff[0].year ,&buff[0].month ,&buff[0].date ,buff[0].arr[ii] - updateWeek);
                }
              }
            }else{
              if(ii + 1 > buff[0].number)
              {
                ii = 0;
                add_date(&buff[0].year ,&buff[0].month ,&buff[0].date ,7 - updateWeek + buff[0].arr[ii]);
              }else{
                ii++;
                add_date(&buff[0].year ,&buff[0].month ,&buff[0].date ,buff[0].arr[ii] - updateWeek);
              }
            }
        }
        buff[0].timePoint = getTimePoint(buff[0].year ,buff[0].month ,buff[0].date ,buff[0].hour ,buff[0].minute ,buff[0].second);
        buff[0].offset = ii;
        //printf("******%d/%d/%d/%d:%d:%d- offset=%d\n",buff[0].year ,buff[0].month ,buff[0].date ,buff[0].hour ,buff[0].minute ,buff[0].second ,buff[0].arr[ii]);
      BubbleSort();
      break;
      }
    }
    
    RTC_Set(yearData ,monthData ,dateData ,hourData ,minuteData ,secondData);
#if 1
    for(uint8_t i = 0; i < rtcDataLen;)
    {
      if(timePoint > buff[i].timePoint)
      {
        switch(buff[i].period)
        {
          case PERIOD_ONCE:
            leftMove();
            rtcDataLen--;
            break;
          case PERIOD_ONE_YEAR:
            // 年份 加一 重新排序
            buff[i].year++;
            buff[i].timePoint = getTimePoint(buff[i].year ,buff[i].month ,buff[i].date ,buff[i].hour ,buff[i].minute ,buff[i].second);
            while(0 == Check_date(buff[i].year ,buff[i].month ,buff[i].date) || buff[i].timePoint <= timePoint)
            {
              buff[i].year++;
              buff[i].timePoint = getTimePoint(buff[i].year ,buff[i].month ,buff[i].date ,buff[i].hour ,buff[i].minute ,buff[i].second);
            }

            i++;
            break;
          case PERIOD_ONE_MONTH:
            // 月份 加一 重新排序
            if(buff[i].month == 12){
              buff[i].year++;
              buff[i].month = 1;
              buff[i].timePoint = getTimePoint(buff[i].year ,buff[i].month ,buff[i].date ,buff[i].hour ,buff[i].minute ,buff[i].second);
              while(0 == Check_date(buff[i].year ,buff[i].month ,buff[i].date) || buff[i].timePoint <= timePoint)
              {
                if(buff[i].month == 12){
                  buff[i].year++;
                  buff[i].month = 1;
                }else{
                  buff[i].month++;
                }
                buff[i].timePoint = getTimePoint(buff[i].year ,buff[i].month ,buff[i].date ,buff[i].hour ,buff[i].minute ,buff[i].second);
              }

            }else{
              if(buff[i].month == 12){
                buff[i].year++;
                buff[i].month = 1;
              }else{
                buff[i].month++;
              }
              buff[i].timePoint = getTimePoint(buff[i].year ,buff[i].month ,buff[i].date ,buff[i].hour ,buff[i].minute ,buff[i].second);
              while(0 == Check_date(buff[i].year ,buff[i].month ,buff[i].date) || buff[i].timePoint <= timePoint)
              {
                if(buff[i].month == 12){
                  buff[i].year++;
                  buff[i].month = 1;
                }else{
                  buff[i].month++;
                }
                buff[i].timePoint = getTimePoint(buff[i].year ,buff[i].month ,buff[i].date ,buff[i].hour ,buff[i].minute ,buff[i].second);
              }
            }
            //printf("%d/%d/%d/%d:%d:%d--\n ",buff[i].year ,buff[i].month ,buff[i].date ,buff[i].hour ,buff[i].minute ,buff[i].second);
            i++;
            break;
          case PERIOD_TWO_WEEK:
            add_date(&buff[i].year ,&buff[i].month ,&buff[i].date ,14);
            buff[i].timePoint = getTimePoint(buff[i].year ,buff[i].month ,buff[i].date ,buff[i].hour ,buff[i].minute ,buff[i].second);
            while(buff[i].timePoint <= timePoint)
            {
              add_date(&buff[i].year ,&buff[i].month ,&buff[i].date ,14);
              buff[i].timePoint = getTimePoint(buff[i].year ,buff[i].month ,buff[i].date ,buff[i].hour ,buff[i].minute ,buff[i].second);
            }
            i++;
            break;
          case PERIOD_ONE_WEEK:
            add_date(&buff[i].year ,&buff[i].month ,&buff[i].date ,7);
            buff[i].timePoint = getTimePoint(buff[i].year ,buff[i].month ,buff[i].date ,buff[i].hour ,buff[i].minute ,buff[i].second);
            while(buff[i].timePoint <= timePoint)
            {
              add_date(&buff[i].year ,&buff[i].month ,&buff[i].date ,7);
              buff[i].timePoint = getTimePoint(buff[i].year ,buff[i].month ,buff[i].date ,buff[i].hour ,buff[i].minute ,buff[i].second);
            }
            i++;
            break;
          case PERIOD_ONE_DATE:
            add_date(&buff[i].year ,&buff[i].month ,&buff[i].date ,1);
            buff[i].timePoint = getTimePoint(buff[i].year ,buff[i].month ,buff[i].date ,buff[i].hour ,buff[i].minute ,buff[i].second);
            while(buff[i].timePoint <= timePoint)
            {
              add_date(&buff[i].year ,&buff[i].month ,&buff[i].date ,1);
              buff[i].timePoint = getTimePoint(buff[i].year ,buff[i].month ,buff[i].date ,buff[i].hour ,buff[i].minute ,buff[i].second);
            }

            i++;
            break;
          case PERIOD_ALARM:
            buff[i].offset++;
            if(buff[i].offset == buff[i].number)
            {
              buff[i].offset = 0;
            }
            if(buff[i].arr[buff[i].offset] >= tweek){
              add_date(&buff[i].year ,&buff[i].month ,&buff[i].date ,buff[i].arr[buff[i].offset] - tweek);
            }else{
              add_date(&buff[i].year ,&buff[i].month ,&buff[i].date ,7 - tweek + buff[i].arr[buff[i].offset]);
            }

            buff[i].timePoint = getTimePoint(buff[i].year ,buff[i].month ,buff[i].date ,buff[i].hour ,buff[i].minute ,buff[i].second);
            while(buff[i].timePoint <= timePoint)
            {
              buff[i].offset++;
              if(buff[i].offset == buff[i].number)
              {
                buff[i].offset = 0;
              }
              if(buff[i].arr[buff[i].offset] >= tweek){
                add_date(&buff[i].year ,&buff[i].month ,&buff[i].date ,buff[i].arr[buff[i].offset] - tweek);
              }else{
                add_date(&buff[i].year ,&buff[i].month ,&buff[i].date ,7 - tweek + buff[i].arr[buff[i].offset]);
              }
              buff[i].timePoint = getTimePoint(buff[i].year ,buff[i].month ,buff[i].date ,buff[i].hour ,buff[i].minute ,buff[i].second);
            }
            //printf("%d/%d/%d/%d:%d:%d--\n ",buff[i].year ,buff[i].month ,buff[i].date ,buff[i].hour ,buff[i].minute ,buff[i].second);
            break;
          case PERIOD_ALARM_ONCE:
            leftMove();
            rtcDataLen--;
            break;
          default:
            i++;
            break;
        }
      }else{
        i++;
      }
    }
#endif
    BubbleSort();        // 排序
    AT_OK;
  }else if(strncmp("AT_GET_PLAN_LIST" ,(const char *)data ,strlen("AT_GET_PLAN_LIST")) == 0){
    if(rtcDataLen == 0){
      uartLock = 0;
      AT_LIST_EMPTY;
      return;
    }
    for(uint8_t i = 0; i < rtcDataLen; i++)
    {
      //printf("next-%d/%d/%d/%d:%d:%d-",buff[i].year ,buff[i].month ,buff[i].date ,buff[i].hour ,buff[i].minute ,buff[i].second);
#if 1
      switch(buff[i].type)
      {
        case AT_SHUTDOWN:
          USB_SEND_DATA("AT_SHUTDOWN=");
          break;
        case AT_ALARM_SHUTDOWN:
          USB_SEND_DATA("AT_ALARM_SHUTDOWN=");
          break;
        case AT_ALARM_POWER_OFF:
          USB_SEND_DATA("AT_ALARM_POWER_OFF=");
          break;
        case AT_ALARM_START_UP:
          USB_SEND_DATA("AT_ALARM_STARTING_UP=");
          break;
        case AT_START_UP:
          USB_SEND_DATA("AT_STARTING_UP=");
          break;
        case AT_POWER_OFF:
          USB_SEND_DATA("AT_POWER_OFF=");
          break;
        default:
          break;
      }
      if(buff[i].period == PERIOD_ALARM || buff[i].period == PERIOD_ALARM_ONCE){
        memset(usbStr, 0, sizeof(usbStr));
        sprintf(usbStr, "%d:%d:%d",buff[i].old_hour ,buff[i].old_minute ,buff[i].old_second);
        USB_SEND_DATA(usbStr);
      }else{
        memset(usbStr, 0, sizeof(usbStr));
        sprintf(usbStr, "%d/%d/%d/%d:%d:%d",buff[i].old_year ,buff[i].old_month ,buff[i].old_date ,buff[i].old_hour ,buff[i].old_minute ,buff[i].old_second);
        USB_SEND_DATA(usbStr);
      }
      switch(buff[i].period)
      {
        case PERIOD_ONCE:
          break;
        case PERIOD_ALARM_ONCE:
          break;
        case PERIOD_ONE_YEAR:
          USB_SEND_DATA("/Y");
          break;
        case PERIOD_ONE_MONTH:
          USB_SEND_DATA("/M");
          break;
        case PERIOD_TWO_WEEK:
          USB_SEND_DATA("/2W");
          break;
        case PERIOD_ONE_WEEK:
          USB_SEND_DATA("/W");
          break;
        case PERIOD_ONE_DATE:
          USB_SEND_DATA("/D");
          break;
        case PERIOD_ALARM:
          USB_SEND_DATA("/");
          memset(usbStr, 0, sizeof(usbStr));
          for(uint8_t j=0; j < buff[i].number; j++) {
            // printf("%d",buff[i].arr[j]);
            sprintf(usbStr+j, "%d",buff[i].arr[j]);
          }
          USB_SEND_DATA(usbStr);
          break;
        default:
          break;
      }
      memset(usbStr, 0, sizeof(usbStr));
      sprintf(usbStr, "--next time-%d/%d/%d/%d:%d:%d\n",buff[i].year ,buff[i].month ,buff[i].date ,buff[i].hour ,buff[i].minute ,buff[i].second);
      USB_SEND_DATA(usbStr);
#endif
    }
  }else if(strncmp("AT_CLEAR_LIST=" ,(const char *)data ,strlen("AT_CLEAR_LIST=")) == 0){
    if(rtcDataLen == 0){
      uartLock = 0;      
      AT_LIST_EMPTY;
      return;
    }
        
    if(0 == analysisString(data ,tDelete ,&tCount3 ,&deleteLen ,&deleteData ,len ,2 ,"\r" ,0 ,20 ,"0" ,"9"))
    {
      uartLock = 0;
      AT_DATA_ERROR;
      return;
    }
    if(0 == deleteAlarm(deleteData))
    {
      uartLock = 0;
      AT_DELETE_ERROR;
      return;
    }
    
    if(rtcDataLen == 0){
      RTC_Set_Alam(2000 ,1 ,1 ,10 ,10 ,0xFFFF);  //清除闹钟事件
    }else{
      // flush_Alarm();
      RTC_Set_Alam(buff[0].year ,buff[0].month ,buff[0].date ,buff[0].hour ,buff[0].minute ,buff[0].second);
    }
    AT_CLEAR_OK;
  }else if(strncmp("AT_CLEAR_LIST_ALL" ,(const char *)data ,strlen("AT_CLEAR_LIST_ALL")) == 0){
    if(rtcDataLen == 0){
      uartLock = 0;      
      AT_LIST_EMPTY;
      return;
    }
    for(uint8_t i = 0; i < rtcDataLen; i++){
      memset(&buff[i] ,0 ,sizeof(buff[0]));
    }
    RTC_Set_Alam(2000 ,1 ,1 ,10 ,10 ,0xFFFF);  //清除闹钟事件
    rtcDataLen = 0;
    AT_CLEAR_OK;
  }else if(strncmp("AT_GET_ELECTRIC" ,(const char *)data ,strlen("AT_GET_ELECTRIC")) == 0){
    // 显示电池当前电压
    // uint16_t volt = voltage();
    // memset(usbStr, 0, sizeof(usbStr));
    // sprintf(usbStr, "voltage = %u mv\n",volt);
    // USB_SEND_DATA(usbStr);

    // 显示电量百分比
    uint8_t batteryLevel = 0;
    I2cGetRegs(&hi2c2, BQ34Z100_G1_I2C_ADDR, (uint8_t)BQ34Z100_STATE_OF_CHARGE, &batteryLevel, 1);
    // 显示电池当前充电状态
    uint8_t data = 0;
    I2cGetRegs(&hi2c1, BQ25792_I2C_ADDR, (uint8_t)BQ25792_CHG_STATUS1, &data, 1);
    data >>= 5;

    memset(usbStr, 0, sizeof(usbStr));
    sprintf(usbStr, "percentage = %u %%\ncharging state = %u \n", batteryLevel, data);
    USB_SEND_DATA(usbStr);
  }else if(strncmp("AT_GET_TIME" ,(const char *)data ,strlen("AT_GET_TIME")) == 0){
    //if(time.tm_week == 0) time.tm_week = 7;
    memset(usbStr, 0, sizeof(usbStr));
    sprintf(usbStr, "%d/%d/%d/%d:%d:%d week = %d\n",time.tm_year, time.tm_month ,time.tm_date ,time.tm_hour, time.tm_minute ,time.tm_second,time.tm_week);
    USB_SEND_DATA(usbStr);
  }else if(strncmp("AT_HELP" ,(const char *)data ,strlen("AT_HELP")) == 0){
    HelpInformation();
  }else{
    AT_ERROR;
  }
  /*
  for(uint8_t i = 0; i < len; i++)
    printf("%c",data[i]);
  printf("\n");
  */
  //printf("len is %d\n",len);


  uartLock = 0;
}
#endif

/* USER CODE END PRIVATE_FUNCTIONS_IMPLEMENTATION */

/**
  * @}
  */

/**
  * @}
  */
