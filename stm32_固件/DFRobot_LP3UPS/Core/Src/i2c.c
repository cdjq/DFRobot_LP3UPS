/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    i2c.c
  * @brief   This file provides code for the configuration
  *          of the I2C instances.
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
#include "i2c.h"

/* USER CODE BEGIN 0 */

unsigned char i2c_data[35];
/* USER CODE END 0 */

I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c2;

/* I2C1 init function */
void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /*
  uint8_t data = 0;
  HAL_StatusTypeDef ret = I2cGetRegs(&hi2c1, BQ25792_I2C_ADDR, (uint8_t)BQ25792_ADC_CTRL, &data, 1);
  if (HAL_OK != ret){
    printf("ret1 = %u\r\n", ret);
    // Error_Handler();
  }

  // printf("data = %#x\r\n", data);
  data |= (1UL << (7));       // start A/D convertion
  // printf("data = %#x\r\n", data);
  ret = I2cSetRegs(&hi2c1, BQ25792_I2C_ADDR, (uint16_t)BQ25792_ADC_CTRL, &data, 1);
  if (HAL_OK != ret){
    printf("ret2 = %u\r\n", ret);
    // Error_Handler();
  }
  */

  /* USER CODE END I2C1_Init 2 */

}
/* I2C2 init function */
void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.ClockSpeed = 100000;
  hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

void HAL_I2C_MspInit(I2C_HandleTypeDef* i2cHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(i2cHandle->Instance==I2C1)
  {
  /* USER CODE BEGIN I2C1_MspInit 0 */

  /* USER CODE END I2C1_MspInit 0 */

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**I2C1 GPIO Configuration
    PB6     ------> I2C1_SCL
    PB7     ------> I2C1_SDA
    */
    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* I2C1 clock enable */
    __HAL_RCC_I2C1_CLK_ENABLE();
  /* USER CODE BEGIN I2C1_MspInit 1 */

  /* USER CODE END I2C1_MspInit 1 */
  }
  else if(i2cHandle->Instance==I2C2)
  {
  /* USER CODE BEGIN I2C2_MspInit 0 */

  /* USER CODE END I2C2_MspInit 0 */

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**I2C2 GPIO Configuration
    PB10     ------> I2C2_SCL
    PB11     ------> I2C2_SDA
    */
    GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* I2C2 clock enable */
    __HAL_RCC_I2C2_CLK_ENABLE();
  /* USER CODE BEGIN I2C2_MspInit 1 */

  /* USER CODE END I2C2_MspInit 1 */
  }
}

void HAL_I2C_MspDeInit(I2C_HandleTypeDef* i2cHandle)
{

  if(i2cHandle->Instance==I2C1)
  {
  /* USER CODE BEGIN I2C1_MspDeInit 0 */

  /* USER CODE END I2C1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_I2C1_CLK_DISABLE();

    /**I2C1 GPIO Configuration
    PB6     ------> I2C1_SCL
    PB7     ------> I2C1_SDA
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_6);

    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_7);

  /* USER CODE BEGIN I2C1_MspDeInit 1 */

  /* USER CODE END I2C1_MspDeInit 1 */
  }
  else if(i2cHandle->Instance==I2C2)
  {
  /* USER CODE BEGIN I2C2_MspDeInit 0 */

  /* USER CODE END I2C2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_I2C2_CLK_DISABLE();

    /**I2C2 GPIO Configuration
    PB10     ------> I2C2_SCL
    PB11     ------> I2C2_SDA
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_10);

    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_11);

  /* USER CODE BEGIN I2C2_MspDeInit 1 */

  /* USER CODE END I2C2_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
/**
 * @fn I2cGetRegs
 * @brief i2c read
 * @param hi2c - i2c总线句柄
 * @param addr - 从机地址
 * @param reg - 寄存器地址
 * @param pbuf - 要读取数据的指针
 * @param len - 要读取数据的字节长度
 * @return 操作状态 HAL_StatusTypeDef
 */
HAL_StatusTypeDef I2cGetRegs(I2C_HandleTypeDef *hi2c, uint8_t addr, uint8_t reg, uint8_t *pbuf, uint16_t len)
{
  HAL_StatusTypeDef ret;
  ret = HAL_I2C_Master_Transmit(hi2c, addr | 1, &reg, 1, 1000);
  if (HAL_OK != ret){
    printf("I2cGetRegs ret = %u\r\n", ret);
    return ret;
  }
  HAL_Delay(10);
  ret = HAL_I2C_Master_Receive(hi2c, addr, pbuf, len, 1000);
  return ret;
}

/**
 * @fn I2cSetRegs
 * @brief i2c write
 * @param hi2c - i2c总线句柄
 * @param addr - 从机地址
 * @param reg - 寄存器地址
 * @param pbuf - 要写入数据的指针
 * @param len - 要写入数据的字节长度
 * @return 操作状态 HAL_StatusTypeDef
 */
HAL_StatusTypeDef I2cSetRegs(I2C_HandleTypeDef *hi2c, uint8_t addr, uint8_t reg, uint8_t *pbuf, uint16_t len)
{
  HAL_StatusTypeDef ret;
  uint8_t buf[255]={0};
  buf[0]=reg;
  for(int i=1;i<len+1;i++)
    buf[i]=pbuf[i-1];
  ret = HAL_I2C_Master_Transmit(hi2c, addr | 1, buf, len+1, 1000);
  return ret;
}

/**
 * @fn i2c_read
 * @brief i2c read
 * @param slave_addr - 从机地址
 * @param reg - 寄存器地址
 * @param NBytes - 要读取数据的字节长度
 * @return 返回读取数据的指针
 */
unsigned char * i2c_read(unsigned char slave_addr, unsigned char reg, unsigned char NBytes)
{
  I2cGetRegs(&hi2c2, slave_addr<<1, reg, i2c_data, NBytes);
  return i2c_data;
}

/**
 * @fn i2c_write
 * @brief i2c write
 * @param slave_addr - 从机地址
 * @param reg - 寄存器地址
 * @param data_to_write - 要写入的数据指针
 * @param NBytes - 要写入数据的字节长度
 * @return 返回0表示操作成功，否则失败
 */
int i2c_write(unsigned char slave_addr, unsigned char reg, unsigned char *data_to_write, unsigned char NBytes)
{
  HAL_StatusTypeDef ret = I2cSetRegs(&hi2c2, slave_addr<<1, reg, data_to_write, NBytes);
  if (HAL_OK != ret){
    printf("ret2 = %u\r\n", ret);
    return -1;
  }
  return 0;
}

/* USER CODE END 1 */
