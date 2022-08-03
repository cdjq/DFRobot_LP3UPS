/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f1xx_it.c
  * @brief   Interrupt Service Routines.
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
#include "main.h"
#include "stm32f1xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "gpio.h"
#include "i2c.h"
#include "usbd_cdc_if.h"
#include "rtc.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
static uint8_t _chargeState, _batteryLevel, _timerCount;
// uint32_t hyytimerTick = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern PCD_HandleTypeDef hpcd_USB_FS;
extern RTC_HandleTypeDef hrtc;
extern TIM_HandleTypeDef htim3;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M3 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */
  // printf("NMI_Handler\n");

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
  while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */
  // printf("HardFault_Handler\n");

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */
  // printf("MemManage_Handler\n");

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Prefetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */
  // printf("BusFault_Handler\n");

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */
  // printf("UsageFault_Handler\n");

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */
  // printf("SVC_Handler\n");

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */
  // printf("DebugMon_Handler\n");

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */
  // printf("PendSV_Handler\n");

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F1xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f1xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles RTC global interrupt.
  */
void RTC_IRQHandler(void)
{
  /* USER CODE BEGIN RTC_IRQn 0 */
  printf("RTC_IRQHandler\n");

  /* USER CODE END RTC_IRQn 0 */
  HAL_RTCEx_RTCIRQHandler(&hrtc);
  /* USER CODE BEGIN RTC_IRQn 1 */

  /* USER CODE END RTC_IRQn 1 */
}

/**
  * @brief This function handles EXTI line0 interrupt.
  */
void EXTI0_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI0_IRQn 0 */
  printf("EXTI0_IRQHandler GPIO_PIN_0\n");

  /* USER CODE END EXTI0_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
  /* USER CODE BEGIN EXTI0_IRQn 1 */

  /* USER CODE END EXTI0_IRQn 1 */
}

/**
  * @brief This function handles EXTI line2 interrupt.
  */
void EXTI2_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI2_IRQn 0 */
  uint8_t data = 0;
  HAL_StatusTypeDef ret = I2cGetRegs(&hi2c1, BQ25792_I2C_ADDR, (uint8_t)BQ25792_CHG_STATUS1, &data, 1);
  if (HAL_OK != ret) {
    printf("_chargeState ret = %u\r\n", ret);
  }

  _chargeState = data >> 5;
  printf("_chargeState = %u\r\n", _chargeState);
  if(_chargeState){
    HAL_TIM_Base_Start_IT(&htim3);
  }

  /* USER CODE END EXTI2_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);
  /* USER CODE BEGIN EXTI2_IRQn 1 */

  /* USER CODE END EXTI2_IRQn 1 */
}

/**
  * @brief This function handles USB low priority or CAN RX0 interrupts.
  */
void USB_LP_CAN1_RX0_IRQHandler(void)
{
  /* USER CODE BEGIN USB_LP_CAN1_RX0_IRQn 0 */

  /* USER CODE END USB_LP_CAN1_RX0_IRQn 0 */
  HAL_PCD_IRQHandler(&hpcd_USB_FS);
  /* USER CODE BEGIN USB_LP_CAN1_RX0_IRQn 1 */

  /* USER CODE END USB_LP_CAN1_RX0_IRQn 1 */
}

/**
  * @brief This function handles EXTI line[9:5] interrupts.
  */
void EXTI9_5_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI9_5_IRQn 0 */
  if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_8) != 0x00u) {
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_8);
    printf("-----------1\n");
    uint8_t i = 0;
    if(1 == HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_8)) {

      HAL_TIM_Base_Start_IT(&htim3);   // 按下按键就显示电量

      for(i = 0; i < 100; i++) {
        if(1 == HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_8)) {
          HAL_Delay(10);
          continue;
        } else {
          break;
        }
      }
      if(i >= 99) {
        // if(_batteryLevel || _chargeState) {
          _timerCount = 0;
          _powerState = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12) ? GPIO_PIN_RESET : GPIO_PIN_SET;
          powerConfig(_powerState);
          // HAL_GPIO_TogglePin(GPIOB ,GPIO_PIN_12);
          // printf("_powerState = %u\r\n", _powerState);
          // POWER_LED_(_powerState);
          if( (0 == _powerState) && !_chargeState ) {
            HAL_TIM_Base_Stop_IT(&htim3);
            TURNOFF_LED;
          }
        // }
      } else {
        // if(adc_data) {
        // } else {
        //   if(sw_flag == 2) {
        //   } else {
        //     sw_flag = 2; // 鐭�?
        //     time3_enable();
        //   }
        // }
      }
    }
  }

  /* USER CODE END EXTI9_5_IRQn 0 */
  // HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_8);
  /* USER CODE BEGIN EXTI9_5_IRQn 1 */

  /* USER CODE END EXTI9_5_IRQn 1 */
}

/**
  * @brief This function handles TIM3 global interrupt.
  */
void TIM3_IRQHandler(void)
{
  /* USER CODE BEGIN TIM3_IRQn 0 */
  // timerTick++;
  // if(0 == _timerCount) {
  //   showPowerLED(_chargeState, _batteryLevel, timerTick);
  //   printf("timerTick = %u\n", timerTick);
  // } else {
  //   printf("_timerCount = %u\n", _timerCount);
  // }
  _timerCount++;
  showPowerLED(_chargeState, _batteryLevel, _timerCount);

  if(30 < _timerCount) {
    _timerCount = 0;
    HAL_StatusTypeDef ret = I2cGetRegs(&hi2c2, BQ34Z100_G1_I2C_ADDR, (uint8_t)BQ34Z100_STATE_OF_CHARGE, &_batteryLevel, 1);
    if (HAL_OK != ret) {
      printf("_batteryLevel ret = %u\r\n", ret);
    }
    printf("_batteryLevel = %u\r\n", _batteryLevel);
    if(!_chargeState && !HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12)) {
        HAL_TIM_Base_Stop_IT(&htim3);
        TURNOFF_LED;
    } else {
    }
  }

  /* USER CODE END TIM3_IRQn 0 */
  HAL_TIM_IRQHandler(&htim3);
  /* USER CODE BEGIN TIM3_IRQn 1 */

  /* USER CODE END TIM3_IRQn 1 */
}

/**
  * @brief This function handles RTC alarm interrupt through EXTI line 17.
  */
void RTC_Alarm_IRQHandler(void)
{
  /* USER CODE BEGIN RTC_Alarm_IRQn 0 */
  // printf("RTC_Alarm_IRQHandler\n");

  _nowTime time = RTC_Get();
  uint8_t  tweek = time.tm_week;
  uint32_t oldTimePoint = time.tm_oldTimePoint;
  
  switch(buff[0].type)
  {
    case AT_START_UP:
      powerConfig(POWER_OFF);
      HAL_Delay(2);
      USB_SEND_DATA("AT start up\n");
      powerConfig(POWER_ON);
      break;
    case AT_SHUTDOWN:
      USB_SEND_DATA("AT shutdown now\n");
      break;
    case AT_ALARM_START_UP:
      powerConfig(POWER_OFF);
      HAL_Delay(1);
      USB_SEND_DATA("AT alarm start up\n");
      powerConfig(POWER_ON);
      break;
    case AT_ALARM_SHUTDOWN:
      USB_SEND_DATA("AT alarm shutdown now\n");
      break;
    case AT_POWER_OFF:
      USB_SEND_DATA("AT power off\n");
      powerConfig(POWER_OFF);
      break;
    case AT_ALARM_POWER_OFF:
      USB_SEND_DATA("AT alarm power off\n");
      powerConfig(POWER_OFF);
      break;
    default:
      break;
  }

  updateAlarm(tweek ,0,0,0,0,0,0,0);

  while(buff[0].timePoint == oldTimePoint)       // 重复的时间只响应一次
  {
    updateAlarm(tweek ,0,0,0,0,0,0,0);
  }

  // flush_Alarm();
  RTC_Set_Alam(buff[0].year ,buff[0].month ,buff[0].date ,buff[0].hour ,buff[0].minute ,buff[0].second);

  /* USER CODE END RTC_Alarm_IRQn 0 */
  HAL_RTC_AlarmIRQHandler(&hrtc);
  /* USER CODE BEGIN RTC_Alarm_IRQn 1 */

  /* USER CODE END RTC_Alarm_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
