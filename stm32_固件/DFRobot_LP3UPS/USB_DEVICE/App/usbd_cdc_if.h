/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : usbd_cdc_if.h
  * @version        : v2.0_Cube
  * @brief          : Header for usbd_cdc_if.c file.
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
#ifndef __USBD_CDC_IF_H__
#define __USBD_CDC_IF_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "usbd_cdc.h"

/* USER CODE BEGIN INCLUDE */

/* USER CODE END INCLUDE */

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @brief For Usb device.
  * @{
  */

/** @defgroup USBD_CDC_IF USBD_CDC_IF
  * @brief Usb VCP device module
  * @{
  */

/** @defgroup USBD_CDC_IF_Exported_Defines USBD_CDC_IF_Exported_Defines
  * @brief Defines.
  * @{
  */
/* Define size for the receive and transmit buffer over CDC */
#define APP_RX_DATA_SIZE  512
#define APP_TX_DATA_SIZE  512
/* USER CODE BEGIN EXPORTED_DEFINES */
typedef struct 
{
	volatile uint8_t RecFlag;
	volatile uint16_t RecLen;
}RxDef;
extern RxDef USB_COM;
extern uint8_t UserRxBufferFS[APP_RX_DATA_SIZE];//????
extern uint8_t UserTxBufferFS[APP_TX_DATA_SIZE];//????

extern  uint8_t UTART_ANALYSIS[257];
extern  uint8_t uartDataLen;
extern  uint8_t uartFlag;
extern  uint8_t FlashWriteFlag;
extern volatile uint8_t uartLock;
extern  uint8_t rtcDataLen;

#define UART_BUF_SIZE         1       //���?��
#define MAX_UART_LENGTH       258
#define UART_SUCCESS          1
#define UART_ERROR            0

#define MAX_LIST_LEN          20


#define PERIOD_ONCE           10
#define PERIOD_ONE_YEAR       1
#define PERIOD_ONE_MONTH      2
#define PERIOD_TWO_WEEK       3
#define PERIOD_ONE_WEEK       4
#define PERIOD_ONE_DATE       5
#define PERIOD_ALARM          6
#define PERIOD_ALARM_ONCE     7

#define AT_START_UP           1
#define AT_SHUTDOWN           2
#define AT_ALARM_START_UP     3
#define AT_ALARM_SHUTDOWN     4
#define AT_POWER_OFF          5
#define AT_ALARM_POWER_OFF    6


#define AT_DATA_ERROR         USB_SEND_DATA("AT_DATA_ERROR\n")
#define AT_ERROR              USB_SEND_DATA("AT_ERROR\n")

#define AT_TIM_ERROR          USB_SEND_DATA("AT_TIM_ERROR\n")
#define AT_ALARM_REPEAT       USB_SEND_DATA("AT_ALARM_REPEAT\n")
#define AT_OK                 USB_SEND_DATA("AT_OK\n")
#define AT_LIST_FULL_ERROR    USB_SEND_DATA("AT_LIST_FULL_ERROR\n")
#define AT_CLEAR_OK           USB_SEND_DATA("AT_CLEAR_OK\n")
#define AT_DELETE_ERROR       USB_SEND_DATA("AT_DELETE_ERROR\n")
#define AT_LIST_EMPTY         USB_SEND_DATA("AT_LIST_EMPTY\n")

/* USER CODE END EXPORTED_DEFINES */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Exported_Types USBD_CDC_IF_Exported_Types
  * @brief Types.
  * @{
  */

/* USER CODE BEGIN EXPORTED_TYPES */

/* USER CODE END EXPORTED_TYPES */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Exported_Macros USBD_CDC_IF_Exported_Macros
  * @brief Aliases.
  * @{
  */

/* USER CODE BEGIN EXPORTED_MACRO */

/* USER CODE END EXPORTED_MACRO */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Exported_Variables USBD_CDC_IF_Exported_Variables
  * @brief Public variables.
  * @{
  */

/** CDC Interface callback. */
extern USBD_CDC_ItfTypeDef USBD_Interface_fops_FS;

/* USER CODE BEGIN EXPORTED_VARIABLES */

/* USER CODE END EXPORTED_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Exported_FunctionsPrototype USBD_CDC_IF_Exported_FunctionsPrototype
  * @brief Public functions declaration.
  * @{
  */

uint8_t CDC_Transmit_FS(uint8_t* Buf, uint16_t Len);

/* USER CODE BEGIN EXPORTED_FUNCTIONS */
void flush_Alarm(void);
void USB_SEND_DATA(char * str);
void leftMove(void);
uint8_t RTC_Set_Alam(uint16_t syear,uint16_t smon,uint16_t sday,uint16_t hour,uint16_t min,uint16_t sec);
void updateAlarm(uint8_t Week ,uint8_t flag ,uint16_t year ,uint16_t month ,uint16_t date ,uint16_t hour ,uint16_t minute ,uint16_t second);
uint8_t deleteAlarm(uint8_t number);
uint8_t analysisString(uint8_t *data , uint8_t *recv ,uint8_t *offset , uint8_t *recvLen ,uint16_t *lastData ,uint8_t len ,uint8_t strLen ,char *lastString ,uint16_t compare1 ,uint16_t compare2 ,char *string1 ,char *string2);
void Assignment(uint32_t sTimePoint ,uint16_t sYear ,uint8_t sMonth ,uint8_t sDate ,uint8_t sHour ,uint8_t sMinute ,uint8_t sSecond ,uint8_t sType ,uint8_t sPeriod ,uint8_t sOffset ,uint8_t sNumber ,uint8_t *sWeek ,uint8_t sWeekLen);
void Data_Analysis(uint8_t *data ,uint8_t len);

/* USER CODE END EXPORTED_FUNCTIONS */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __USBD_CDC_IF_H__ */

