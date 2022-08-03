/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.h
  * @brief   This file contains all the function prototypes for
  *          the gpio.c file
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
#ifndef __GPIO_H__
#define __GPIO_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */
extern __IO GPIO_PinState _powerState;

#define CHR_CE_PORT   GPIOA
#define CHR_INT_PORT  GPIOB
#define CHR_QON_PORT  GPIOB
#define ALERT_PORT    GPIOB

#define CHR_CE_PIN    GPIO_PIN_7
#define CHR_INT_PIN   GPIO_PIN_2
#define CHR_QON_PIN   GPIO_PIN_1
#define ALERT_PIN     GPIO_PIN_0

#define LED_1_(mode)   HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, (GPIO_PinState)mode)
#define LED_2_(mode)   HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, (GPIO_PinState)mode)
#define LED_3_(mode)   HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, (GPIO_PinState)mode)
#define LED_4_(mode)   HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, (GPIO_PinState)mode)

#define POWER_LED_(mode)   HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, mode)

#define POWER_ON    ((GPIO_PinState)1)
#define POWER_OFF   ((GPIO_PinState)0)

#define LED_1_TOGGLE   HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_2)
#define LED_2_TOGGLE   HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_3)
#define LED_3_TOGGLE   HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_4)
#define LED_4_TOGGLE   HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5)
#define TURNOFF_LED    {LED_1_(0); LED_2_(0); LED_3_(0); LED_4_(0);}

/* USER CODE END Private defines */

void MX_GPIO_Init(void);

/* USER CODE BEGIN Prototypes */
void powerConfig(GPIO_PinState on_off);
void showPowerLED(uint8_t chargeState, uint8_t batteryLevel, uint8_t timerCount);

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ GPIO_H__ */

