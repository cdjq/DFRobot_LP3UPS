/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
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
#include "gpio.h"

/* USER CODE BEGIN 0 */
__IO GPIO_PinState _powerState  = GPIO_PIN_RESET;

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5
                          |GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1|GPIO_PIN_12, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA2 PA3 PA4 PA5
                           PA6 PA7 */
  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5
                          |GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB0 PB2 PB8 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_2|GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PB1 PB12 */
  GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 5, 4);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  HAL_NVIC_SetPriority(EXTI2_IRQn, 5, 4);
  HAL_NVIC_EnableIRQ(EXTI2_IRQn);

  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 5, 4);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

}

/* USER CODE BEGIN 2 */
/**
 * @brief 控制对LP的供电，并附带切换指示LED
 * 
 * @param on_off GPIO_PIN_RESET 断电， GPIO_PIN_SET 上电
 */
void powerConfig(GPIO_PinState on_off)
{
  _powerState = on_off;
  POWER_LED_(_powerState);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, _powerState);
}

/**
 * @fn showPowerLED
 * @brief 显示电量led
 * @param chargeState - 充电状态, 0表示未连接充电器
 * @param batteryLevel - 电池电量, 0~100
 * @param timerCount - 定时器计数值, 区分快闪慢闪
 * @return None
 */
void showPowerLED(uint8_t chargeState, uint8_t batteryLevel, uint8_t timerCount)
{
  switch(batteryLevel / 25) {
    case 0:   // 电池电量 < 25
      LED_1_(0);
      LED_2_(0);
      LED_3_(0);
      if(chargeState) {   // 充电状态
        LED_4_TOGGLE;   // LED快闪 6 HZ
      } else {
        if(10 > batteryLevel) {
          if(timerCount % 2)   // 电量过低, LED慢闪 3 HZ
            LED_4_TOGGLE;
        } else {
          LED_4_(1);
        }
      }
      break;
    case 1:   // 电池电量 < 50
      LED_1_(0);
      LED_2_(0);
      if(chargeState) {
        LED_3_TOGGLE;
      } else {
        LED_3_(1);
      }
      LED_4_(1);
      break;
    case 2:   // 电池电量 < 75
      LED_1_(0);
      if(chargeState) {
        LED_2_TOGGLE;
      } else {
        LED_2_(1);
      }
      LED_3_(1);
      LED_4_(1);
      break;
    case 3:   // 电池电量 < 100
      if(chargeState) {
        LED_1_TOGGLE;
      } else {
        LED_1_(1);
      }
      LED_2_(1);
      LED_3_(1);
      LED_4_(1);
      break;
    case 4:   // 电池电量 = 100
      LED_1_(1);
      LED_2_(1);
      LED_3_(1);
      LED_4_(1);
      break;
    default:
      break;
  }
}

/* USER CODE END 2 */
