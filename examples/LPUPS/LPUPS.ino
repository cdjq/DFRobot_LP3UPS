/*!
 * @file  LPUPS.ino
 * @brief  
 * @details  
 * @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license  The MIT License (MIT)
 * @author  [qsjhyy](yihuan.huang@dfrobot.com)
 * @version  V1.0
 * @date  2022-11-23
 * @url  https://github.com/DFRobot/DFRobot_LPUPS
 */
#include <DFRobot_LPUPS.h>
#include <HIDPowerDevice.h>

DFRobot_LPUPS_I2C LPUPS(&Wire, /*I2CAddr*/ UPS_I2C_ADDRESS);

#define DATA_LEN_MAX   0X22U
uint8_t regBuf[DATA_LEN_MAX] = {0};
sChargerStatus0_t chargerStatus0;
sChargerStatus1_t chargerStatus1;
sProchotStatus0_t prochotStatus0;
sProchotStatus1_t prochotStatus1;
uint16_t systemPower = 0, inputVoltage = 0;
uint16_t dischargeCurrent = 0, chargeCurrent = 0;
uint16_t CMPINVoltage = 0, inputCurrent = 0;
uint16_t batteryVoltage = 0, systemVoltage = 0;
char outputBuf[512];   // 打印缓冲区

#define MIN_UPDATE_INTERVAL   26   // USB-HID 最小上报间隔
int iIntTimer = 0;   // 更新间隔计数器

// 字符串常量
const char STRING_DEVICE_CHEMISTRY[] PROGMEM = "Li-ion";   // Li-ion
const char STRING_OEM_VENDOR[] PROGMEM = "MyCoolUPS";
const char STRING_SERIAL[] PROGMEM = "UPS100";   // UPS100

const byte bDeviceChemistry = IDEVICECHEMISTRY;   // Index of a string descriptor containing the battery’s chemistry.
const byte bOEMVendor = IOEMVENDOR;

uint16_t iPresentStatus = 0, iPreviousStatus = 0;   // 现在和之前的设备状态

byte bRechargable = 1;   // Rechargeable Battery (1)/Not Rechargeable Battery (0)
byte bCapacityMode = 2;   // 数据手册中, 2表示电池容量以 % 为单位

// 物理参数
const uint16_t iConfigVoltage = 13800;   // Nominal value of the voltage.
uint16_t iVoltage =1300, iPrevVoltage = 0;
uint16_t iRunTimeToEmpty = 0, iPrevRunTimeToEmpty = 0;
uint16_t iAvgTimeToFull = 7200;
uint16_t iAvgTimeToEmpty = 7200;   // 12
uint16_t iRemainTimeLimit = 600;   // 1
/* Writing this value immediately shuts down (i.e., turns off) the output
   for a period equal to the indicated number of seconds in 
   DelayBeforeReboot, after which time the output is started. */
int16_t  iDelayBe4Reboot = -1;
/* Writing this value shuts down (i.e., turns off) either the output after
  the indicated number of seconds, or sooner if the batteries become depleted. */
int16_t  iDelayBe4ShutDown = -1;

byte iAudibleAlarmCtrl = 2; // 1 - Disabled, 2 - Enabled, 3 - Muted

// 符合 ACPI(高级配置和电源接口) 的参数
const byte iDesignCapacity = 100;
byte iWarnCapacityLimit = 10; // warning at 10% 
byte iRemnCapacityLimit = 5; // low at 5% 
const byte bCapacityGranularity1 = 1; // Battery capacity granularity between low and warning.
const byte bCapacityGranularity2 = 1; // Battery capacity granularity between warning and full.
byte iFullChargeCapacity = 100;

byte iRemaining =0, iPrevRemaining=0;
int iRes=0;

void setup(void)
{
  Serial.begin(115200);

  // Init the sensor
  while( NO_ERR != LPUPS.begin() ){
    Serial.println("Communication with device failed, please check connection");
    delay(3000);
  }
  Serial.println("Begin ok!");

  pinMode(9, OUTPUT);   // 电量指示LED, 绿色
  pinMode(10, OUTPUT);   // 电量指示LED, 红色
  pinMode(13, OUTPUT);   // 输出刷新1秒, 表示arduino周期正在运行, 蓝色

  initPowerDevice();   // HIDPowerDevice 的初始化

}

void loop()
{
  /************ 获取充电芯片的数据并打印 ****************************/
  printChargeData();

  /*********** 计量单位，测量单位 ****************************/
  bool bCharging, bACPresent, bDischarging;   // 正在充电, 有交流电提供, 正在放电
  if(regBuf[CS32_I2C_CHARGER_STATUS_REG + 1] >> 7) {   // 检测是否有外部供电
    bACPresent = true;
    if(regBuf[CS32_I2C_CHARGER_STATUS_REG + 1] & 0x3F) {   // 检测是否正在充电
      bCharging = true;
    } else {
      bCharging = false;
    }
    bDischarging = false;
  } else {
    bACPresent = false;
    bCharging = false;
    if(regBuf[CS32_I2C_CHARGER_STATUS_REG + 1] & 0x3F) {   // 检测是否正在放电
      bDischarging = true;
    } else {
      bDischarging = false;
    }
  }

  iRemaining = regBuf[CS32_I2C_ADC_VBAT_REG + 1] * 100 / 255;
  iRunTimeToEmpty = (float)iAvgTimeToEmpty*iRemaining/100;
  Serial.print("iRemaining = ");
  Serial.println(iRemaining);
  Serial.print("iRunTimeToEmpty = ");
  Serial.println(iRunTimeToEmpty);
  iRemaining = 99;
  iRunTimeToEmpty = 3600;

  digitalWrite(9, LOW);   // 打开 绿色 LED灯;
  digitalWrite(10, LOW);   // 打开 红色 LED灯;
  if (( iRemaining >= 0)&&( iRemaining <= 25)){   // 25%-红灯亮-0%
    digitalWrite(9, HIGH);   // 关闭 绿色 LED灯;
  } else if (( iRemaining > 25) && ( iRemaining < 75)){   // 75%-红绿灯都亮-25%
  
  } else if (( iRemaining >= 75) && ( iRemaining <= 100)){   // 100%-绿灯亮-75%
    digitalWrite(10, HIGH);   // 关闭 红色 LED灯;
  }

  /******* 根据获取的充电芯片数据 刷新要USB-HID上报的值 ************/
  flashReportedData();

  /************ 延时 ***************************************/
  delay(1000);
  iIntTimer++;
  digitalWrite(13, LOW);   // 打开 蓝色 LED灯;
  delay(1000);
  iIntTimer++;
  digitalWrite(13, HIGH);   // 关掉 蓝色 LED灯;

  /************ 检查一下我们是否还在线 ******************/
  /************ 批量发送或中断 ***********************/
  if((iPresentStatus != iPreviousStatus) || (iRemaining != iPrevRemaining) || 
     (iRunTimeToEmpty != iPrevRunTimeToEmpty) || (iIntTimer > MIN_UPDATE_INTERVAL) ) {

    // 12 INPUT OR FEATURE(required by Windows)
    PowerDevice.sendReport(HID_PD_REMAININGCAPACITY, &iRemaining, sizeof(iRemaining));
    if(bDischarging) PowerDevice.sendReport(HID_PD_RUNTIMETOEMPTY, &iRunTimeToEmpty, sizeof(iRunTimeToEmpty));
    iRes = PowerDevice.sendReport(HID_PD_PRESENTSTATUS, &iPresentStatus, sizeof(iPresentStatus));

    if(iRes <0 ) {   // 上报返回值, 小于0: 表示 与主机的通信丢失
      pinMode(13, INPUT);
    }
    else{
      pinMode(13, OUTPUT);
    }

    iIntTimer = 0;   // 上报时间间隔计数清零
    iPreviousStatus = iPresentStatus;   // 保存新的设备状态
    iPrevRemaining = iRemaining;   // 保存新的电池剩余容量
    iPrevRunTimeToEmpty = iRunTimeToEmpty;   // 保存新的预估电池用空时间计数
  }

  Serial.print("iRemaining = ");   // 剩余电量的百分比
  Serial.println(iRemaining);
  Serial.print("iRunTimeToEmpty = ");   // 多久用完剩余电量
  Serial.println(iRunTimeToEmpty);
  Serial.print("iRes = ");   // 上报返回值, 小于0: 表示 与主机的通信丢失
  Serial.println(iRes);
  Serial.println();
}

void initPowerDevice(void)
{
  PowerDevice.begin();

  // 序列号是以特殊方式设置的，因为它形成了Arduino端口名称
  PowerDevice.setSerial(STRING_SERIAL);

  // 用于调试目的。
  PowerDevice.setOutput(Serial);

  // usb上报参数设置
  PowerDevice.setFeature(HID_PD_PRESENTSTATUS, &iPresentStatus, sizeof(iPresentStatus));

  PowerDevice.setFeature(HID_PD_RUNTIMETOEMPTY, &iRunTimeToEmpty, sizeof(iRunTimeToEmpty));
  PowerDevice.setFeature(HID_PD_AVERAGETIME2FULL, &iAvgTimeToFull, sizeof(iAvgTimeToFull));
  PowerDevice.setFeature(HID_PD_AVERAGETIME2EMPTY, &iAvgTimeToEmpty, sizeof(iAvgTimeToEmpty));
  PowerDevice.setFeature(HID_PD_REMAINTIMELIMIT, &iRemainTimeLimit, sizeof(iRemainTimeLimit));
  PowerDevice.setFeature(HID_PD_DELAYBE4REBOOT, &iDelayBe4Reboot, sizeof(iDelayBe4Reboot));
  PowerDevice.setFeature(HID_PD_DELAYBE4SHUTDOWN, &iDelayBe4ShutDown, sizeof(iDelayBe4ShutDown));

  PowerDevice.setFeature(HID_PD_RECHARGEABLE, &bRechargable, sizeof(bRechargable));
  PowerDevice.setFeature(HID_PD_CAPACITYMODE, &bCapacityMode, sizeof(bCapacityMode));
  PowerDevice.setFeature(HID_PD_CONFIGVOLTAGE, &iConfigVoltage, sizeof(iConfigVoltage));
  PowerDevice.setFeature(HID_PD_VOLTAGE, &iVoltage, sizeof(iVoltage));

  PowerDevice.setStringFeature(HID_PD_IDEVICECHEMISTRY, &bDeviceChemistry, STRING_DEVICE_CHEMISTRY);
  PowerDevice.setStringFeature(HID_PD_IOEMINFORMATION, &bOEMVendor, STRING_OEM_VENDOR);

  PowerDevice.setFeature(HID_PD_AUDIBLEALARMCTRL, &iAudibleAlarmCtrl, sizeof(iAudibleAlarmCtrl));

  PowerDevice.setFeature(HID_PD_DESIGNCAPACITY, &iDesignCapacity, sizeof(iDesignCapacity));
  PowerDevice.setFeature(HID_PD_FULLCHRGECAPACITY, &iFullChargeCapacity, sizeof(iFullChargeCapacity));
  PowerDevice.setFeature(HID_PD_REMAININGCAPACITY, &iRemaining, sizeof(iRemaining));
  PowerDevice.setFeature(HID_PD_WARNCAPACITYLIMIT, &iWarnCapacityLimit, sizeof(iWarnCapacityLimit));
  PowerDevice.setFeature(HID_PD_REMNCAPACITYLIMIT, &iRemnCapacityLimit, sizeof(iRemnCapacityLimit));
  PowerDevice.setFeature(HID_PD_CPCTYGRANULARITY1, &bCapacityGranularity1, sizeof(bCapacityGranularity1));
  PowerDevice.setFeature(HID_PD_CPCTYGRANULARITY2, &bCapacityGranularity2, sizeof(bCapacityGranularity2));
}

void printChargeData(void)
{
  /**
   *  获取芯片数据
   *  regBuf - 数据缓存buf
   */
  LPUPS.getChipData(regBuf);

  /*************** CS32_I2C_CHARGER_STATUS_REG ~ CS32_I2C_PROCHOT_STATUS_REG ***************/
  memcpy(&chargerStatus0, &regBuf[CS32_I2C_CHARGER_STATUS_REG], sizeof(regBuf[CS32_I2C_CHARGER_STATUS_REG]));
  memcpy(&chargerStatus1, &regBuf[CS32_I2C_CHARGER_STATUS_REG + 1], sizeof(regBuf[CS32_I2C_CHARGER_STATUS_REG + 1]));
  memcpy(&prochotStatus0, &regBuf[CS32_I2C_PROCHOT_STATUS_REG], sizeof(regBuf[CS32_I2C_PROCHOT_STATUS_REG]));
  memcpy(&prochotStatus1, &regBuf[CS32_I2C_PROCHOT_STATUS_REG + 1], sizeof(regBuf[CS32_I2C_PROCHOT_STATUS_REG + 1]));
  memset(outputBuf, 0, sizeof(outputBuf));
  sprintf(outputBuf, "Charge status register 0 = %#x\r\n"
                     "Charge status register 1 = %#x\r\n"
                     "Prochot status register 0 = %#x\r\n"
                     "Prochot status register 1 = %#x\r\n", 
          regBuf[CS32_I2C_CHARGER_STATUS_REG], regBuf[CS32_I2C_CHARGER_STATUS_REG + 1], regBuf[CS32_I2C_PROCHOT_STATUS_REG], regBuf[CS32_I2C_PROCHOT_STATUS_REG + 1]);
  Serial.print(outputBuf);

  /*************** CS32_I2C_ADC_PSYS_REG ~ CS32_I2C_ADC_VSYS_REG ***************/
  // PSYS: Full range: 3.06 V, LSB: 12 mV
  systemPower = regBuf[CS32_I2C_ADC_PSYS_REG] * 12;
  // VBUS: Full range: 3.2 V - 19.52 V, LSB: 64 mV
  inputVoltage = 3200 + regBuf[CS32_I2C_ADC_VBUS_REG] * 64;
  // IDCHG: Full range: 32.512 A, LSB: 256 mA
  dischargeCurrent = regBuf[CS32_I2C_ADC_IDCHG_REG] * 256;
  // ICHG: Full range 8.128 A, LSB: 64 mA
  chargeCurrent = regBuf[CS32_I2C_ADC_ICHG_REG] * 64;
  // CMPIN: Full range 3.06 V, LSB: 12 mV
  CMPINVoltage = regBuf[CS32_I2C_ADC_CMPIN_REG] * 12;
  // POR State - IIN: Full range: 12.75 A, LSB: 50 mA
  inputCurrent = regBuf[CS32_I2C_ADC_IIN_REG] * 50;
  // VBAT: Full range : 2.88 V - 19.2 V, LSB 64 mV
  batteryVoltage = 2880 + regBuf[CS32_I2C_ADC_VBAT_REG] * 64;
  // VSYS: Full range: 2.88 V - 19.2 V, LSB: 64 mV
  systemVoltage = 2880 + regBuf[CS32_I2C_ADC_VSYS_REG] * 64;
  memset(outputBuf, 0, sizeof(outputBuf));
  sprintf(outputBuf, "8-bit Digital Output of System Power = %u mV\r\n"
                     "8-bit Digital Output of Input Voltage = %u mV\r\n"
                     "8-bit digital output of battery discharge current = %u mA\r\n"
                     "8-bit digital output of battery charge current = %u mA\r\n"
                     "8-bit digital output of CMPIN voltage = %u mV\r\n"
                     "8-bit digital output of input current = %u mA\r\n"
                     "8-bit digital output of battery voltage = %u mV\r\n"
                     "8-bit digital output of system voltage = %u mV\r\n", 
          systemPower, inputVoltage, dischargeCurrent, chargeCurrent, CMPINVoltage, inputCurrent, batteryVoltage, systemVoltage);
  Serial.print(outputBuf);

}

void flashReportedData(void)
{
  // 充电状态
  if(bCharging) 
    bitSet(iPresentStatus,PRESENTSTATUS_CHARGING);
  else
    bitClear(iPresentStatus,PRESENTSTATUS_CHARGING);

  // 交流电供电
  if(bACPresent) 
    bitSet(iPresentStatus,PRESENTSTATUS_ACPRESENT);
  else
    bitClear(iPresentStatus,PRESENTSTATUS_ACPRESENT);

  // 充满电
  if(iRemaining == iFullChargeCapacity) 
    bitSet(iPresentStatus,PRESENTSTATUS_FULLCHARGE);
  else 
    bitClear(iPresentStatus,PRESENTSTATUS_FULLCHARGE);

  // Discharging
  if(bDischarging) {   // 掉电，未充电
    bitSet(iPresentStatus,PRESENTSTATUS_DISCHARGING);
    // if(iRemaining < iRemnCapacityLimit) bitSet(iPresentStatus,PRESENTSTATUS_BELOWRCL);   // 低于剩余容量限制

    // 超过剩余 时间/电量 限制
    if(iRunTimeToEmpty < iRemainTimeLimit) 
      bitSet(iPresentStatus, PRESENTSTATUS_RTLEXPIRED);
    else
      bitClear(iPresentStatus, PRESENTSTATUS_RTLEXPIRED);

  }
  else {
    bitClear(iPresentStatus,PRESENTSTATUS_DISCHARGING);
    bitClear(iPresentStatus, PRESENTSTATUS_RTLEXPIRED);   // 充电中 清相关标志位
  }

  // 关机请求
  if(iDelayBe4ShutDown > 0 ) {
      bitSet(iPresentStatus, PRESENTSTATUS_SHUTDOWNREQ);
      Serial.println("shutdown requested");
  }
  else
    bitClear(iPresentStatus, PRESENTSTATUS_SHUTDOWNREQ);

  // 即将关机
  if((iPresentStatus & (1 << PRESENTSTATUS_SHUTDOWNREQ)) || 
     (iPresentStatus & (1 << PRESENTSTATUS_RTLEXPIRED))) {
    bitSet(iPresentStatus, PRESENTSTATUS_SHUTDOWNIMNT);   // - 即将关机
    Serial.println("shutdown imminent");
  }
  else
    bitClear(iPresentStatus, PRESENTSTATUS_SHUTDOWNIMNT);

  bitSet(iPresentStatus ,PRESENTSTATUS_BATTPRESENT);   // - 电源 BATT
}
