/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    i2c.h
  * @brief   This file contains all the function prototypes for
  *          the i2c.c file
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
#ifndef __I2C_H__
#define __I2C_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern I2C_HandleTypeDef hi2c1;

extern I2C_HandleTypeDef hi2c2;

/* USER CODE BEGIN Private defines */
#define BQ25792_I2C_ADDR   ((uint8_t)(0x6B << 1))
#define BQ34Z100_G1_I2C_ADDR   ((uint8_t)(0x55 << 1))

/**
 * @enum eBQ25792Reg_t
 * @brief BQ25792çš„I2Cå¯„å­˜å™?
 */
typedef enum {
  BQ25792_VSYSMIN        = 0x00,
  BQ25792_VREG           = 0x01,
  BQ25792_ICHG           = 0x03,
  BQ25792_VINDPM         = 0x05,
  BQ25792_IINDPM         = 0x06,
  BQ25792_PRE_CHARGE     = 0x08,
  BQ25792_TERM_REG       = 0x09,
  BQ25792_RECHG          = 0x0A,
  BQ25792_VOTG           = 0x0B,
  BQ25792_IOTG           = 0x0D,
  BQ25792_EN_TCR         = 0x0E,
  BQ25792_EN_CCR0        = 0x0F,
  BQ25792_EN_CCR1        = 0x10,
  BQ25792_EN_CCR2        = 0x11,
  BQ25792_EN_CCR3        = 0x12,
  BQ25792_EN_CCR4        = 0x13,
  BQ25792_EN_CCR5        = 0x14,
  BQ25792_RSRV_REG       = 0x15,
  BQ25792_TEMP_REG       = 0x16,
  BQ25792_NTC_REG0       = 0x17,
  BQ25792_NTC_REG1       = 0x18,
  BQ25792_ICO_CLR        = 0x19,
  BQ25792_CHG_STATUS0    = 0x1B,
  BQ25792_CHG_STATUS1    = 0x1C,
  BQ25792_CHG_STATUS2    = 0x1D,
  BQ25792_CHG_STATUS3    = 0x1E,
  BQ25792_CHG_STATUS4    = 0x1F,
  BQ25792_FAULT_STATUS0  = 0x20,
  BQ25792_FAULT_STATUS1  = 0x21,
  BQ25792_CH_FLAGREG0    = 0x22,
  BQ25792_CH_FLAGREG1    = 0x23,
  BQ25792_CH_FLAGREG2    = 0x24,
  BQ25792_CH_FLAGREG3    = 0x25,
  BQ25792_FAULT_FLAGREG0 = 0x26,
  BQ25792_FAULT_FLAGREG1 = 0x27,
  BQ25792_CH_MASKREG0    = 0x28,
  BQ25792_CH_MASKREG1    = 0x29,
  BQ25792_CH_MASKREG2    = 0x2A,
  BQ25792_CH_MASKREG3    = 0x2B,
  BQ25792_FAULT_MASKREG0 = 0x2C,
  BQ25792_FAULT_MASKREG1 = 0x2D,
  BQ25792_ADC_CTRL       = 0x2E,
  BQ25792_ADC_DISABLE0   = 0x2F,
  BQ25792_ADC_DISABLE1   = 0x30,
  BQ25792_IBUS_ADC       = 0x31,
  BQ25792_IBAT_ADC       = 0x33,
  BQ25792_VBUS_ADC       = 0x35,
  BQ25792_VAC1_ADC       = 0x37,
  BQ25792_VAC2_ADC       = 0x39,
  BQ25792_VBAT_ADC       = 0x3B,
  BQ25792_VBAT_ADC2      = 0x3C,
  BQ25792_VSYS_ADC       = 0x3D,
  BQ25792_TS_ADC         = 0X3F,
  BQ25792_TDIE_ADC       = 0x41,
  BQ25792_DP_ADC         = 0x43,
  BQ25792_DN_ADC         = 0x45,
  BQ25792_DPDM_ADC       = 0x47,
  BQ25792_PART_INFO      = 0x48
} eBQ25792Reg_t;

//commands
#define BQ34Z100_CONTROL                0x00
#define BQ34Z100_STATE_OF_CHARGE        0x02
#define BQ34Z100_MAX_ERROR              0x03
#define BQ34Z100_REMAINING_CAPACITY     0x04
#define BQ34Z100_FULL_CHARGE_CAPACITY   0x06
#define BQ34Z100_VOLTAGE                0x08
#define BQ34Z100_AVERAGE_CURRENT        0x0A
#define BQ34Z100_TEMPERATURE            0x0C
#define BQ34Z100_CURRENT                0x10

//extended data commands
#define BQ34Z100_AVAILABLE_ENERGY       0x24    //10mW/h
#define BQ34Z100_AVERAGE_POWER          0X26    //10mW
#define BQ34Z100_INTERNAL_TEMPERATURE   0x2A    //0.1k
#define BQ34Z100_PACK_CONFIGURATION     0x3A
#define BQ34Z100_DESIGN_CAPACITY        0x3C    //0X3C



//control subcommands
#define BQ34Z100_CONTROL_STATUS   0x0000 //return the status of key features
#define BQ34Z100_DEVICE_TYPE      0x0001 //return the device type of 0x100(indicate BQ34Z100-G1)
#define BQ34Z100_CHEM_ID          0x0008 // return the chemID value
#define BQ34Z100_BOARD_OFFSET     0X0009 // force the device to measure and store board offset
#define BQ34Z100_CC_OFFSET        0x000A // force the device to measure the internal cc offset
#define BQ34Z100_CC_OFFSET_SAVE   0x000B // force the device to store the internal cc offset
#define BQ34Z100_SEALED           0x0020 // places the device in sealed mode
#define BQ34Z100_IT_ENABLE        0x0021 // enable the impedance track algorith
#define BQ34Z100_CAL_ENABLE       0x002D // toggle calibration mode enable
#define BQ34Z100_RESET            0x0041 // reset the fuel guage
#define BQ34Z100_ENTER_CAL        0x0081 // enter the calibration mode
#define BQ34Z100_EXIT_CAL         0x0080 // exit the calibration mode
#define BQ34Z100_OFFSET_CAL       0x0082 // report internal cc offset in calibration mode

/* USER CODE END Private defines */

void MX_I2C1_Init(void);
void MX_I2C2_Init(void);

/* USER CODE BEGIN Prototypes */
HAL_StatusTypeDef I2cSetRegs(I2C_HandleTypeDef *hi2c, uint8_t addr, uint8_t reg, uint8_t *pbuf, uint16_t len);
HAL_StatusTypeDef I2cGetRegs(I2C_HandleTypeDef *hi2c, uint8_t addr, uint8_t reg, uint8_t *pbuf, uint16_t len);

// i2c read 
unsigned char * i2c_read(unsigned char slave_addr, unsigned char reg, unsigned char NBytes);

// i2c write
int i2c_write(unsigned char slave_addr, unsigned char reg, unsigned char *data_to_write, unsigned char NBytes);

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __I2C_H__ */

