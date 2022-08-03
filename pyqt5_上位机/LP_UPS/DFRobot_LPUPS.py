# -*- coding: utf-8 -*-
"""!
  @file DFRobot_LPUPS.py
  @brief 使用本程序和LP_UPS3进行通信，接口为USB-CDC
  @details 获取电池信息，定时开关机
  @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
  @license   The MIT license (MIT)
  @author [qsjhyy](yihuan.huang@dfrobot.com)
  @version  V1.0
  @date  2022-08-01
  @https://github.com/DFRobot/DFRobot_LPUPS
"""
import sys
import os
import threading
import time

import serial
import serial.tools.list_ports
from PyQt5 import QtCore
from PyQt5.QtCore import QDateTime
from PyQt5.QtWidgets import QWidget, QApplication, QMessageBox, QDialog

from untitled import Ui_Form, Ui_Dialog, Ui_Dialog_task


class lp_usb_cdc(object):
    """!
        @brief Define lp usb cdc basic class
    """

    def __init__(self, com):
        """!
            @brief Module usb cdc init
            @param com - usb cdc port
        """
        # usb-cdc 通信锁
        self._R_lock = threading.Lock()

        self._ser = com
        self.open()

    def __del__(self):
        """!
            @brief 关闭已打开的串口，销毁对象，释放内存
        """
        self.close()
        del self._ser

    def open(self):
        """!
            @brief 打开已关闭的串口
        """
        if not self._ser.isOpen:
            self._ser.open()

    def close(self):
        """!
            @brief 关闭已打开的串口
        """
        if self._ser.isOpen:
            self._ser.close()

    def read_data(self, data):
        """!
            @brief 发送指令并读取回复的数据
            @param data - 要发送的AT指令
            @return 读到的usb-cdc数据 字符串
        """
        with self._R_lock:  # 为usb-cdc读写操作 加锁
            if self._ser:
                try:
                    # print(data)
                    self._ser.write(data.encode())  # 写入指令
                    for i in range(0, 80):  # 等待答复
                        if self._ser.in_waiting:
                            break
                        else:
                            time.sleep(0.0001)
                    read = b''
                    while self._ser.in_waiting:  # 完全接收本次答复
                        read += self._ser.read(self._ser.in_waiting)
                        time.sleep(0.0001)
                    if "shutdown now\n" in bytes.decode(read):
                        print(bytes.decode(read))
                        # os.system('shutdown /s /t 0')
                    return bytes.decode(read)
                except Exception as e:  # 串口不正确或者已断开
                    mainWidget.change_com_button_4()
                    mainWidget.flash_com_button_3()
                    print(str(e).replace(":", ":\n"))
                    msg_box = QMessageBox(QMessageBox.Critical, '串口不正确或者已断开', str(e).replace(":", ":\n"))
                    msg_box.exec_()
                    print(str(e).replace(":", ":\n"))


# 时间补零（列如 2022/8/1/9:31:39 -> 2022/08/01/09:31:39）
def dateTime_padding_zero(_dateTime):
    temp_list = _dateTime.split("/")
    temp_time = '{0:0>4s}/{1:0>2s}/{2:0>2s}/'.format(temp_list[0], temp_list[1], temp_list[2])
    temp_time += time_padding_zero(temp_list[3])
    # print(f'-dateTime_padding_zero temp_time = {temp_time}')
    return temp_time


# 时间补零（列如 16:1:1 -> 16:01:01）
def time_padding_zero(_time):
    temp_list = _time.split(":")
    temp_time = '{0:0>2s}:{1:0>2s}:{2:0>2s}'.format(temp_list[0], temp_list[1], temp_list[2])
    # print(f'-time_padding_zero temp_time = {temp_time}')
    return temp_time


# usb获取数据线程
class usb_Thread(QtCore.QThread):

    def __init__(self, parent=None):
        super(usb_Thread, self).__init__(parent)
        self.data = ""
        self.data_is_new = 0

    def usb_read(self, data):
        self.data = data
        self.data_is_new = 1

    # 当USB数据接收完成，发送一个信号给主线程渲染界面
    def run(self):
        while 1:
            time.sleep(0.0001)
            if self.data_is_new:
                mainWidget.my_com.read_data(self.data)
                self.data_is_new = 0


# 一个刷新时间的线程类
class time_Thread(QtCore.QThread):
    def __init__(self):
        super(time_Thread, self).__init__()
        self.count = 5  # 第一次初始化电量信息

    # 线程执行函数
    def run(self):
        while 1:
            if 5 <= self.count:  # 每6秒刷新一次电量信息
                self.count = 0
                # 电池电量及充电状态显示
                if mainWidget.my_com:
                    e_info = mainWidget.my_com.read_data("AT_GET_ELECTRIC\r")  # 电池信息
                    if e_info:
                        str_if = e_info.split(" ")
                        # print(str_if)
                        if 8 == len(str_if):
                            mainWidget.progressBar.setValue(int(str_if[2]))
                            if 7 == int(str_if[6]):
                                mainWidget.label_3.setText("已充满")
                            elif 0 == int(str_if[6]):
                                mainWidget.label_3.setText("电量")
                            else:
                                mainWidget.label_3.setText("充电中")
            self.count += 1

            # 时钟显示
            _time = QDateTime.currentDateTime()  # 获取现在的时间
            if mainWidget.checkBox.isChecked() and mainWidget.my_com:
                _time = mainWidget.my_com.read_data("AT_GET_TIME\r")
                if _time:
                    str_lt = _time.split(" week = ")
                    _time = QtCore.QDateTime.fromString(dateTime_padding_zero(str_lt[0]), "yyyy/MM/dd/hh:mm:ss")
            time_date = _time.toString('yyyy-MM-dd dddd')  # 设置显示日期的格式
            time_time = _time.toString('hh:mm:ss')  # 设置显示时间的格式
            mainWidget.label_4.setText(time_date)
            mainWidget.label.setText(time_time)
            time.sleep(1)  # 每秒刷新一次


class main(Ui_Form, QWidget):
    # usb_read_signal = QtCore.pyqtSignal(str)

    # 初始化主界面及通信
    def __init__(self):
        self.my_com = None
        self._port_list = []
        self.task_list = []
        self.task_list_description = []
        self.task_type_list = ["AT_STARTING_UP", "AT_ALARM_STARTING_UP", "AT_SHUTDOWN", "AT_ALARM_SHUTDOWN",
                               "AT_POWER_OFF", "AT_ALARM_POWER_OFF"]
        self.cycle_type_list = ["D", "W", "2W", "M", "Y"]

        # 调用父类的init()初始化函数
        super().__init__()
        # 初始化 untitled Ui_Form
        self.setupUi(self)

        # 打开串口前 无法使用的控件失能
        self.ctrl_key(False)
        # 扫描com口
        self.flash_com_button_3()
        # 连接信号与槽
        self.connect_signal()

        # 启动时钟刷新线程
        self.my_time_thread = time_Thread()
        self.my_time_thread.start()

        # 启动usb-cdc读线程
        # self.my_usb_thread = usb_Thread()
        # self.my_usb_thread.start()
        # self.usb_read_signal.connect(self.my_usb_thread.usb_read)

    # 初始的使能/使能按键
    def ctrl_key(self, mode):
        self.pushButton.setEnabled(mode)
        # self.pushButton_3.setEnabled(mode)
        # self.pushButton_4.setEnabled(mode)
        self.pushButton_6.setEnabled(mode)
        self.pushButton_7.setEnabled(mode)
        self.checkBox.setHidden(not mode)
        self.listWidget.setEnabled(mode)
        if not mode:  # 如果是统一失能, 列如关闭串口
            self.pushButton_2.setEnabled(False)
            self.pushButton_5.setEnabled(False)

    # 连接信号与槽函数
    def connect_signal(self):
        self.pushButton.clicked.connect(self.new_task_button_1)  # 新建任务按钮 1, 0, 1, 1
        self.pushButton_2.clicked.connect(self.delete_task_button_2)  # 任务删除按钮 1, 1, 1, 1
        self.pushButton_3.clicked.connect(self.flash_com_button_3)  # 端口列表刷新按钮
        self.pushButton_4.clicked.connect(self.change_com_button_4)  # 端口控制按钮
        self.pushButton_5.clicked.connect(self.editing_task_button_5)  # 任务编辑按钮 1, 2, 1, 1
        self.pushButton_6.clicked.connect(self.flash_task_button_6)  # 任务刷新按钮 1, 3, 1, 1
        self.pushButton_7.clicked.connect(self.change_time_button_7)  # 时钟设置按钮
        self.checkBox.clicked.connect(self.change_time_checkBox)  # 时钟源复选框

    # 合成AT指令
    def synthetic_AT_command(self, _dialog):
        task_type = _dialog.comboBox.currentIndex() << 1  # 指令类型 的数字下标
        cycle_type = _dialog.comboBox_2.currentIndex()
        cmd = ""
        if 1 < cycle_type:  # 带 年月日 的任务类型
            cmd += self.task_type_list[task_type]
            cmd += _dialog.dateTimeEdit.dateTime().toString('=yyyy/MM/dd/hh:mm:ss')
            if 2 != cycle_type:  # 不是单次任务, 是按 年 月 日 周 循环的任务
                cmd += "/" + self.cycle_type_list[cycle_type - 3]  # 循环类型 的数字下标
        else:  # 不带 年月日 的任务类型
            cmd += self.task_type_list[task_type + 1]
            cmd += _dialog.timeEdit.time().toString('=hh:mm:ss')
            if 1 == cycle_type:  # 指定 星期 循环的类型
                temp = "/"
                if _dialog.checkBox.isChecked():
                    temp += "1"
                if _dialog.checkBox_2.isChecked():
                    temp += "2"
                if _dialog.checkBox_3.isChecked():
                    temp += "3"
                if _dialog.checkBox_4.isChecked():
                    temp += "4"
                if _dialog.checkBox_5.isChecked():
                    temp += "5"
                if _dialog.checkBox_6.isChecked():
                    temp += "6"
                if _dialog.checkBox_7.isChecked():
                    temp += "7"
                if 1 == len(temp):
                    msg_box = QMessageBox(QMessageBox.Warning, '注意', "未选择要循环的星期，任务循环类型默认为单次！")
                    msg_box.exec_()
                else:
                    cmd += temp
        cmd += "\r"
        return cmd

    # 解析AT指令
    def analysis_AT_command(self, str_task, _dialog):
        str_ts = str_task.split("=")   # ['AT_STARTING_UP', '2022/8/1/9:32:22--next time-2022/8/1/9:32:22']
        temp = self.task_type_list.index(str_ts[0])   # 任务(功能)类型, 6种
        _dialog.comboBox.setCurrentIndex(temp >> 1)   # 开机, 关机, 断电
        description_str = "任务: " + _dialog.comboBox.currentText() + "; 循环: "   # 解析描述任务
        old_cmd = str_ts[0] + "="   # 老任务指令补零并提取
        cycle_type = 0   # 循环类型
        str_ts_1 = str_ts[1].split("--next time-")   # ['2022/8/1/9:32:22', '2022/8/1/9:32:22']
        str_ts_2 = str_ts_1[0].split("/")   # ['2022', '8', '1', '9:32:22'] 或 ['13:27:33', '1234567']
        week_type = ""   # 按周循环的说明
        if temp % 2:  # 不带 年月日 的任务类型
            # 提取下次激活时间(hh:mm:ss) 并格式化(time_padding_zero) 存入 timeEdit
            str_time = time_padding_zero(str_ts_2[0])
            old_cmd += str_time
            _time = QtCore.QDateTime.fromString(str_time, "hh:mm:ss")
            _dialog.timeEdit.setDateTime(_time)

            if 2 == len(str_ts_2):  # 不是单次任务, 是按 指定星期 循环的任务
                old_cmd += "/" + str_ts_2[1]
                week_type = "(" + str_ts_2[1] + ")"
                cycle_type = 1
                if "1" in str_ts_2[1]:
                    _dialog.checkBox.setChecked(True)
                if "2" in str_ts_2[1]:
                    _dialog.checkBox_2.setChecked(True)
                if "3" in str_ts_2[1]:
                    _dialog.checkBox_3.setChecked(True)
                if "4" in str_ts_2[1]:
                    _dialog.checkBox_4.setChecked(True)
                if "5" in str_ts_2[1]:
                    _dialog.checkBox_5.setChecked(True)
                if "6" in str_ts_2[1]:
                    _dialog.checkBox_6.setChecked(True)
                if "7" in str_ts_2[1]:
                    _dialog.checkBox_7.setChecked(True)
        else:  # 带 年月日 的任务类型
            # 提取下次激活时间(yyyy/MM/dd/hh:mm:ss) 并格式化(dateTime_padding_zero) 存入 dateTimeEdit
            str_dateTime = dateTime_padding_zero(str_ts_1[1])
            old_cmd += str_dateTime
            _dateTime = QtCore.QDateTime.fromString(str_dateTime, "yyyy/MM/dd/hh:mm:ss")
            _dialog.dateTimeEdit.setDateTime(_dateTime)

            if 5 == len(str_ts_2):  # 不是单次任务, 是按 年 月 日 周 循环的任务
                cycle_type = self.cycle_type_list.index(str_ts_2[-1]) + 3
                old_cmd += "/" + self.cycle_type_list[cycle_type - 3]
            else:
                cycle_type = 2

        _dialog.comboBox_2.setCurrentIndex(cycle_type)   # 循环类型
        description_str += _dialog.comboBox_2.currentText() + week_type + "; \t任务下次触发时间: " + str_ts_1[1]
        # print(f'-description_str =  {description_str}')
        old_cmd += "\r"
        # print(f'-old_cmd =  {old_cmd}')
        return description_str, old_cmd

    # 按键1按下触发 新建任务
    def new_task_button_1(self):
        if 20 <= self.listWidget.count():
            msg_box = QMessageBox(QMessageBox.Critical, '错误', "最多只能存在20个任务！")
            msg_box.exec_()
            return

        # 创建QDialog对象
        dialog = QDialog()
        date_dialog = Ui_Dialog_task()
        date_dialog.setupUi(dialog)
        dialog.exec_()

        if dialog.result():
            cmd = self.synthetic_AT_command(date_dialog)  # 根据用户输入合成指令
            relt = self.send_cmd(cmd)
            # print([cmd])
            # print(f'-relt =  {relt}')
            if "AT_OK\n" == relt:
                _str = "新建任务成功"
            else:
                _str = "新建任务失败，请确认输入日期迟于当前日期！"
            msg_box = QMessageBox(QMessageBox.Information, '操作结果', _str)
            msg_box.exec_()

            self.flash_task_button_6()  # 刷新任务列表

    # 按键2按下触发 删除选中任务
    def delete_task_button_2(self):
        msg_box = QMessageBox(QMessageBox.Question, '删除任务',
                              '你确定删除当前已选中的任务吗？', QMessageBox.Yes | QMessageBox.No)
        msg_box.exec_()  # 执行弹出框
        if msg_box.result() == QMessageBox.Yes:
            _str = "删除成功"
            _task_list = self.listWidget.selectedIndexes()  # 获取当前所有已选条目信息

            if len(_task_list) == self.listWidget.count():  # 当前已选中了所有任务
                if "AT_CLEAR_OK\n" != self.send_cmd("AT_CLEAR_LIST_ALL\r"):
                    _str = "删除出现错误，请稍后再试！"
            else:
                for _task in reversed(_task_list):
                    cmd = "AT_CLEAR_LIST=" + str(_task.row() + 1) + "\r"
                    # print(cmd)
                    relt = self.send_cmd(cmd)
                    if "AT_CLEAR_OK\n" != relt:
                        _str = "删除出现错误，请稍后再试！"
                        # print(f'-relt =  {relt}')
                        break

            msg_box = QMessageBox(QMessageBox.Information, '操作结果', _str)
            msg_box.exec_()

            self.flash_task_button_6()  # 刷新任务列表

    # 按键3按下触发 刷新串口, 在下拉列表显示
    def flash_com_button_3(self):
        self.comboBox.clear()  # 清空之前的条例
        self._port_list = list(serial.tools.list_ports.comports())
        for i in self._port_list:
            self.comboBox.addItem(i.description)  # i.device i.hwid

    # 按键4按下触发 打开串口或关闭串口
    def change_com_button_4(self):
        if "打开端口" == self.pushButton_4.text():
            self.open_com()
            if self.my_com:
                self.ctrl_key(True)
                self.pushButton_4.setText("关闭端口")
                self.my_time_thread.count = 5  # 让线程直接初始化电量信息
                self.flash_task_button_6()  # 刷新任务列表
        elif "关闭端口" == self.pushButton_4.text():
            self.ctrl_key(False)
            self.pushButton_4.setText("打开端口")
            self.progressBar.reset()
            self.label_3.setText("未连接")
            self.close_com()

    # 按键5按下触发 编辑所选任务
    def editing_task_button_5(self):
        current_index = self.listWidget.currentIndex().row()   # 当前选择的任务序号
        str_task = self.task_list[current_index]   # 当前选择的任务

        # 创建QDialog对象
        dialog = QDialog()
        date_dialog = Ui_Dialog_task()
        date_dialog.setupUi(dialog)

        description_str, old_cmd = self.analysis_AT_command(str_task, date_dialog)   # 解析任务指令

        dialog.exec_()   # 显示编辑对话框

        if dialog.result():
            # print(f'-old_cmd =  {[old_cmd]}')
            cmd = self.synthetic_AT_command(date_dialog)  # 根据用户输入合成指令
            # print(f'-cmd =  {[cmd]}')
            if old_cmd != cmd:   # 若配置发生改变则发送指令
                del_cmd = "AT_CLEAR_LIST=" + str(current_index + 1) + "\r"
                relt = self.send_cmd(del_cmd)   # 将旧任务删除
                if "AT_CLEAR_OK\n" != relt:
                    _str = "任务修改出现错误，请稍后再试！"
                time.sleep(0.1)
                relt = self.send_cmd(cmd)   # 添加编辑后的新任务
                if "AT_OK\n" == relt:
                    _str = "任务保存成功！"
                else:
                    _str = "任务保存失败！"
            else:
                _str = "没有检测到用户更改任务！"
            msg_box = QMessageBox(QMessageBox.Information, '操作结果', _str)
            msg_box.exec_()

            self.flash_task_button_6()  # 刷新任务列表

    # 按键6按下触发 刷新任务列表
    def flash_task_button_6(self):
        self.listWidget.clear()
        _task = self.send_cmd("AT_GET_PLAN_LIST\r")  # 获取任务列表
        while "percentage" in _task:
            _task = self.send_cmd("AT_GET_PLAN_LIST\r")  # 获取任务列表
        self.task_list = _task.split("\n")
        self.task_list.pop(-1)
        # print(self.task_list)
        if "AT_LIST_EMPTY" != self.task_list[0]:  # 如果任务列表不为空
            # 创建QDialog对象
            dialog = QDialog()
            date_dialog = Ui_Dialog_task()
            date_dialog.setupUi(dialog)
            self.task_list_description = []
            for i in self.task_list:
                description_str, old_cmd = self.analysis_AT_command(i, date_dialog)   # 解析任务指令
                description_str = str(self.task_list.index(i) + 1) + "、" + description_str
                self.task_list_description.append(description_str)
            dialog.close()
            self.listWidget.addItems(self.task_list_description)

    # 按键7按下触发 更改UPS时钟
    def change_time_button_7(self):
        _date = ""
        _str = ""
        _str1 = ""
        _str2 = ""
        if self.checkBox.isChecked():  # 按下 更改时钟
            # # 创建QDialog对象
            dialog = QDialog()
            date_dialog = Ui_Dialog()
            date_dialog.setupUi(dialog)
            dialog.exec_()
            if dialog.result():
                _time = date_dialog.dateTimeEdit.dateTime()
                _date = _time.toString('yyyy/MM/dd/hh:mm:ss')  # 设置显示日期的格式
                _str1 = "自定义LP_UPS时钟成功！"
                _str2 = "自定义LP_UPS时钟失败，请稍后再试！"
            else:
                return
        else:  # 按下 同步时钟
            msg_box = QMessageBox(QMessageBox.Question, '退出',
                                  '你确定将当前系统时钟同步到LP_UPS吗？', QMessageBox.Yes | QMessageBox.No)
            msg_box.exec_()  # 执行弹出框
            if msg_box.result() == QMessageBox.Yes:
                _time = QDateTime.currentDateTime()  # 获取现在的时间
                _date = _time.toString('yyyy/MM/dd/hh:mm:ss')  # 设置显示日期的格式
                _str1 = "同步时钟已完成！"
                _str2 = "同步时钟失败，请稍后再试！"
            else:
                return

        relt = self.send_cmd("AT_SET_TIME=" + _date + "\r")
        if "AT_OK\n" == relt:  # 同步时钟到LP_UPS
            _str = _str1
            self.flash_task_button_6()  # 刷新任务列表
        else:
            _str = _str2
        msg_box = QMessageBox(QMessageBox.Information, '操作结果', _str)
        msg_box.exec_()

    # 复选框选择触发 更换时钟源
    def change_time_checkBox(self):
        if self.checkBox.isChecked():
            self.pushButton_7.setText("更改时钟")
            self.pushButton_7.setToolTip('自定义LP_UPS的时钟')
        else:
            self.pushButton_7.setText("同步时钟")
            self.pushButton_7.setToolTip('将系统时钟同步到LP_UPS')

    # 在主线程调用com口时停止刷新时间的线程
    def send_cmd(self, cmd):
        self.my_time_thread.wait(1)
        return self.my_com.read_data(cmd)

    # 打开串口
    def open_com(self):
        com = self._port_list[self.comboBox.currentIndex()].device
        try:
            self.my_com = lp_usb_cdc(serial.Serial(com))
            h_info = self.send_cmd("AT_HELP\r")  # 帮助信息
            # print(h_info)
            if not h_info:
                self.close_com()
                msg_box = QMessageBox(QMessageBox.Critical, '串口未正确响应', "请选择正确串口, 重新尝试!")
                msg_box.exec_()
            # self.usb_read_signal.emit("AT_HELP\r")
        except Exception as e:  # 串口已被占用, 或者已断开连接
            self.close_com()
            msg_box = QMessageBox(QMessageBox.Critical, '错误', str(e).replace(":", ":\n"))
            msg_box.exec_()

    # 关闭串口
    def close_com(self):
        del self.my_com
        self.my_com = None
        # print("-close_com")


# 按间距中的绿色按钮以运行脚本。
if __name__ == '__main__':
    # usb_data = ""

    app = QApplication(sys.argv)

    # 初始化 QWidget
    mainWidget = main()
    # 显示 mainWidget
    mainWidget.show()

    sys.exit(app.exec())
