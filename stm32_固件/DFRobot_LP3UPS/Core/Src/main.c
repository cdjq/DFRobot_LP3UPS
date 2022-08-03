/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "i2c.h"
#include "rtc.h"
#include "tim.h"
#include "usart.h"
#include "usb_device.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usbd_cdc_if.h"
#include "FlashRW.h"
#include "battery.h"
// #include<math.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void USB_Status_Init(void);

// 加入以下代码,支持printf函数,而不用选择use MicroLIB
#if 1
  #pragma import(__use_no_semihosting)
  // 标准库需要的支持函数
  struct __FILE
  {
    int handle;
  };
  
  FILE __stdout;
  // 定义_sys_exit()以避免使用半主机模式
  void _sys_exit(int x)
  {
    x = x;
  }
  
  // 重定义fputc函数
  int fputc(int ch, FILE *f)
  {      
    while((USART1->SR&0X40)==0){}   // 循环发,直到发完
      USART1->DR = (uint8_t) ch;
    return ch;
  }
#endif

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();
  NVIC_SetPriority(SysTick_IRQn, 3);   // 设置更高的滴答优先级，使得它能在一些中断里面使用

  /* USER CODE BEGIN SysInit */
  USB_Status_Init();
  MX_USART1_UART_Init();

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USB_DEVICE_Init();
  MX_I2C1_Init();
  MX_I2C2_Init();
  MX_TIM3_Init();
  MX_RTC_Init();
  /* USER CODE BEGIN 2 */

  // HAL_GPIO_WritePin(GPIOB ,GPIO_PIN_12, GPIO_PIN_SET);
  RTC_Set(2022, 7, 26, 17, 30, 00);
  while(0)
  {
    if(USB_COM.RecFlag==1) {
      if(UserRxBufferFS[0] == 0x40) {
        UserRxBufferFS[4]++;  //就是串口调试时BC后面的计数
        UserRxBufferFS[5] = '\n';
        CDC_Transmit_FS(UserRxBufferFS, 6);
        USB_COM.RecFlag=0;
      }
    }
  }
  // HAL_GPIO_WritePin(GPIOA ,GPIO_PIN_6, GPIO_PIN_SET);
  // HAL_GPIO_WritePin(GPIOB ,GPIO_PIN_12, GPIO_PIN_SET);
  // HAL_TIM_Base_Start_IT(&htim3);
  // RTC_TimeTypeDef myTime;
  // myTime.Hours = 25;
  // myTime.Minutes = 70;
  // myTime.Seconds = 70;
  // HAL_RTC_SetTime(&hrtc, &myTime, RTC_FORMAT_BIN);
  // while(1) {
  //   HAL_RTC_GetTime(&hrtc, &myTime, RTC_FORMAT_BIN);
  //   printf("myTime.Hours = %u\r\n", myTime.Hours);
  //   printf("myTime.Minutes = %u\r\n", myTime.Minutes);
  //   printf("myTime.Seconds = %u\r\n", myTime.Seconds);
  //   HAL_Delay(1000);
  // }
  //verify and set all the parameter. Alert when fail to set
  // int ret_val;
  // ret_val = gauge_verify_and_calibrate();
  // if(ret_val == -1) {
  //   printf("gauge_verify_and_calibrate failed\n");
  // }
  // autocalibrate();
    // uint8_t buf[100] = {0};
    // uint8_t buf1[100] = {0};
  // while (buff[rtcDataLen].count)
  // {
  //   rtcDataLen 
  // }
  // struct uart_RTC_Time hyy_bufff[20] = {0};
  FlashReadBuff(DEVICE_LOG_ADDRESS, (uint8_t *)buff, sizeof(buff));
  rtcDataLen = buff[0].count;
  for (size_t i = 1; i < 20; i++)
  {
    if(rtcDataLen < buff[i].count) {   // 遍历之前的任务数
      if (0xFF != buff[i].count) {   // 若Flash未写入 是0XFF
        rtcDataLen = buff[i].count;
      }
    }
    // printf("--\nbuff[%u].timePoint = %#x\r\n", i, buff[i].timePoint);
    // printf("buff.offset = %#x\r\n", buff[i].offset);
    // printf("buff.type = %#x\r\n", buff[i].type);
    // printf("buff.number = %#x\r\n", buff[i].number);
    // printf("buff.count = %#x\r\n", buff[i].count);
    // printf("buff.arr[0] = %#x\r\n", buff[i].arr[0]);
    // printf("buff.year = %#x\r\n", buff[i].year);
    // printf("buff.month = %#x\r\n", buff[i].month);
    // printf("buff.date = %#x\r\n", buff[i].date);
    // printf("buff.hour = %#x\r\n", buff[i].hour);
    // printf("buff.minute = %#x\r\n++\n", buff[i].minute);
    // printf("hyy_bufff[%u].timePoint = %#x\r\n", i, hyy_bufff[i].timePoint);
    // printf("hyy_bufff.offset = %#x\r\n", hyy_bufff[i].offset);
    // printf("hyy_bufff.type = %#x\r\n", hyy_bufff[i].type);
    // printf("hyy_bufff.number = %#x\r\n", hyy_bufff[i].number);
    // printf("hyy_bufff.count = %#x\r\n", hyy_bufff[i].count);
    // printf("hyy_bufff.arr[0] = %#x\r\n", hyy_bufff[i].arr[0]);
    // HAL_Delay(1000);
  }
  rtcDataLen += 1;   // 真实任务数为 序号加一
  printf("rtcDataLen = %x\r\n", rtcDataLen);
  RTC_Set_Alam(buff[0].year ,buff[0].month ,buff[0].date ,buff[0].hour ,buff[0].minute ,buff[0].second);
  FlashWriteFlag = 0;   // 写flash标志初始化

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

		if(USB_COM.RecFlag == 1) {
      Data_Analysis(UTART_ANALYSIS ,USB_COM.RecLen);
      USB_COM.RecLen = 0;
      USB_COM.RecFlag = 0;
      memset(UTART_ANALYSIS, 0 ,255);
		}
    flush_Alarm();
		// printf("1\n");
    
    if(1 == FlashWriteFlag) {
      printf("FlashWriteBuff FlashWriteFlag = %u\n", FlashWriteFlag);
      FlashWriteFlag = 0;
      FlashWriteBuff(DEVICE_LOG_ADDRESS, (uint8_t *)buff, sizeof(buff));
      // HAL_Delay(1000);
      // FlashReadBuff(DEVICE_LOG_ADDRESS, (uint8_t *)hyy_bufff, sizeof(hyy_bufff));
      for (size_t i = 0; i < 3; i++)
      {
        printf("--\nbuff[%u].timePoint = %#x\r\n", i, buff[i].timePoint);
        printf("buff.offset = %#x\r\n", buff[i].offset);
        printf("buff.type = %#x\r\n", buff[i].type);
        printf("buff.number = %#x\r\n", buff[i].number);
        printf("buff.count = %#x\r\n", buff[i].count);
        printf("buff.arr[0] = %#x\r\n", buff[i].arr[0]);
        // printf("hyy_bufff[%u].timePoint = %#x\r\n", i, hyy_bufff[i].timePoint);
        // printf("hyy_bufff.offset = %#x\r\n", hyy_bufff[i].offset);
        // printf("hyy_bufff.type = %#x\r\n", hyy_bufff[i].type);
        // printf("hyy_bufff.number = %#x\r\n", hyy_bufff[i].number);
        // printf("hyy_bufff.count = %#x\r\n", hyy_bufff[i].count);
        // printf("hyy_bufff.arr[0] = %#x\r\n++\n", hyy_bufff[i].arr[0]);
        // HAL_Delay(1000);
      }
    }
    // buf[0] += 1;
    // buf[49] += 2;
    // buf[99] += 3;
    // STMFLASH_Write(0x08010000, (uint32_t *)buf, 50);
    // STMFLASH_Read(0x08010000, (uint32_t *)buf1, 50);
    // printf("2---buf1[0] = %u; buf1[49] = %u; buf1[99] = %u\n", buf1[0], buf1[49], buf1[99]);
    
    
    // HAL_Delay(1000);


    /* USER CODE BEGIN 3 */

#if 0
    int ret_val;

    RTC_Get();

    ret_val = internal_temperature();
    if(ret_val == -1) {
        printf("Error internal temperature\n");
    }
    float internal_temp = (float)ret_val*0.1-273.15;
    printf("\ninternal_temp data = %0.2f C\r\n\n", internal_temp);

    ret_val = voltage();
    if(ret_val == -1) {
        printf("Error write to file : voltage\n");
    }
    uint16_t volt = (uint16_t)ret_val;
    printf("volt data = %d mV\r\n", volt);
    if (9300 >= volt) {
      HAL_GPIO_WritePin(GPIOB ,GPIO_PIN_12, GPIO_PIN_RESET);
    }

    ret_val = current();
    if(ret_val == -1) {
      printf("Error current_value\n");
    }
    int16_t current_value = (int16_t)ret_val;
    printf("current_value data = %d mA\r\n\n", current_value);

    HAL_StatusTypeDef ret;
    uint8_t buf[2] = {0};
    ret = I2cGetRegs(&hi2c2, BQ34Z100_G1_I2C_ADDR, (uint8_t)0x02, buf, 2);
    if (HAL_OK != ret) {
      printf("BQ34Z100_G1_I2C_ADDR ret = %u\r\n", ret);
    }
    printf("electric quantity = %u%%\r\n", buf[0]);
    printf("MAX Error = %u%%\r\n\n", buf[1]);
    for(uint8_t i=0; i<2; i++) {
      ret = I2cGetRegs(&hi2c2, BQ34Z100_G1_I2C_ADDR, (uint8_t)0x04+i*2, buf, 2);
      if (HAL_OK != ret) {
        printf("BQ34Z100_G1_I2C_ADDR ret = %u\r\n", ret);
      }
      printf("BQ34Z100_G1_I2C_ADDR data = %d mAh\r\n", (int16_t)(CONCAT_BYTES(buf[1], buf[0])));
    }

    printf("\n++++++++++++++++++++++++++++\r\n\n");

    uint8_t data = 0;
    ret = I2cGetRegs(&hi2c1, BQ25792_I2C_ADDR, (uint8_t)BQ25792_CHG_STATUS1, &data, 1);
    if (HAL_OK != ret) {
      printf("ret1 = %u\r\n", ret);
    }

    data >>= 5;
    printf("charging state = %u\r\n", data);

    data = 0;
    ret = I2cGetRegs(&hi2c1, BQ25792_I2C_ADDR, (uint8_t)BQ25792_ADC_CTRL, &data, 1);
    if (HAL_OK != ret){
      printf("ret1 = %u\r\n", ret);
    }

    data |= (1UL << (7));       // start A/D convertion
    ret = I2cSetRegs(&hi2c1, BQ25792_I2C_ADDR, (uint16_t)BQ25792_ADC_CTRL, &data, 1);
    if (HAL_OK != ret){
      printf("ret2 = %u\r\n", ret);
    }

    // uint8_t buf[2] = {0};
    memset(buf, 0, sizeof(buf));
    ret = I2cGetRegs(&hi2c1, BQ25792_I2C_ADDR, (uint8_t)BQ25792_VBUS_ADC, buf, 2);
    if (HAL_OK != ret) {
      printf("main ret = %u\r\n", ret);
      // Error_Handler();
    }
    printf("VBUS_ADC data = %d mV\r\n", (int16_t)(CONCAT_BYTES(buf[0], buf[1])));

    memset(buf, 0, sizeof(buf));
    // printf("-----data = %u\r\n", CONCAT_BYTES(buf[0], buf[1]));
    ret = I2cGetRegs(&hi2c1, BQ25792_I2C_ADDR, (uint8_t)BQ25792_VSYS_ADC, buf, 2);
    if (HAL_OK != ret) {
      printf("main ret = %u\r\n", ret);
      // Error_Handler();
    }
    printf("VSYS_ADC data = %u mV\r\n", (int16_t)(CONCAT_BYTES(buf[0], buf[1])));

    memset(buf, 0, sizeof(buf));
    // printf("-----data = %u\r\n", CONCAT_BYTES(buf[0], buf[1]));
    ret = I2cGetRegs(&hi2c1, BQ25792_I2C_ADDR, (uint8_t)BQ25792_VBAT_ADC, buf, 2);
    if (HAL_OK != ret) {
      printf("main ret = %u\r\n", ret);
      // Error_Handler();
    }
    printf("VBAT_ADC data = %u mV\r\n", (int16_t)(CONCAT_BYTES(buf[0], buf[1])));

    printf("------------\r\n\n");
    HAL_Delay(1000);

    // showPowerLED(1, 99, 1);

#endif

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_USB;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
/**
 * @brief 用来复位USB口, 即让USB的两个PIN保持一段时间低电平。
 * @return None
 */
void USB_Status_Init(void)
{  
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11|GPIO_PIN_12, GPIO_PIN_RESET);

  /*Configure GPIO pin : PA11 PA12 */  
  GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;  
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  HAL_Delay(10);
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  printf("404\r\n");
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
