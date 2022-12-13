# DFRobot_LPUPS
* [中文版](./README_CN.md)

一款LP的UPS，为LP的电源设备提供对应驱动。

![产品实物图](./resources/images/LPUPS.png)


## Product Link (https://www.dfrobot.com/search-lpups.html)
    SKU: DFR0682


## Table of Contents

* [Summary](#summary)
* [Installation](#installation)
* [Methods](#methods)
* [Compatibility](#compatibility)
* [History](#history)
* [Credits](#credits)


## Summary

* 通过I2C获取LPUPS的电池信息, 通过USB-HID将这些电池信息上报到LP。


## Installation

There two methods:

1. To use this library, first download the library file, paste it into the \Arduino\libraries directory, then open the examples folder and run the demo in the folder.
2. Search the DFRobot_LPUPS library from the Arduino Software Library Manager and download it.


## Methods

```C++

  /**
   * @fn begin
   * @brief Init function
   * @return int type, indicates returning init status
   * @retval 0 NO_ERROR
   * @retval -1 ERR_DATA_BUS
   * @retval -2 ERR_IC_VERSION
   */
  virtual int begin(void);

  /**
   * @fn getChipData
   * @brief 获取芯片数据
   * @param regBuf 
   * @return None
   */
  void getChipData(uint8_t * regBuf);

```


## Compatibility

MCU                | Work Well    | Work Wrong   | Untested    | Remarks
------------------ | :----------: | :----------: | :---------: | :----:
Arduino Leonardo   |      √       |              |             |


## History

- 2022/11/23 - Version 1.0.0 released.


## Credits

Written by qsjhyy(yihuan.huang@dfrobot.com), 2022. (Welcome to our [website](https://www.dfrobot.com/))

