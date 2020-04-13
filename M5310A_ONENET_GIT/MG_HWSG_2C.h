/*
  1-9
  hwsg rs232   function  .h   

  3-24  完善类
  4-2 免密码设置
  4-7 GIT还在吧
*/

#ifndef HWSG_2C_H
#define HWSG_2C_H

#include <ctime>

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif


#define _HWSG_VERSION "1.20.4" // software version of this library

// 握手标志  连续两次去  回一个
#define _HWSG_GETTEM_CMD0  0xC0
#define _HWSG_GETPAR_CMD0  0xD0
#define _HWSG_SETPAR_CMD0  0xE0
#define _HWSG_RESET_CMD0   0xF0

// HWSG 取温度 uart 返回状态  
#define HWSG_UART_OK 0x00
#define HWSG_UART_TIMEOUT 0xFF
#define HWSG_UART_BADPACKET 0xFE

// Define GPIO of  UART ： RXTX is ESP32 的 RXTX
#define M5310_RX3 3 //  origion lib is  rx1  tx3 ,because my faule to rhe wrong gpio  default is HardwareSerial 0
#define M5310_TX1 1 //  注意，引脚关系有点绕：驱动默认的TX0 对应 ESP32的引脚GPIO1 对应 DEV——BOARD板子的TX0插针 对应ESP32WROOM模组的35号孔

#define DIS_RX9 9   //  to diwen lcd diaplay ,  default is HardwareSerial 1
#define DIS_TX10 10 //  以此类推

#define HWSG_RX16 16 // to  hwsg uart  ,default is HardwareSerial 2
#define HWSG_TX17 17

#define HWSG_CMD_Uploadtem0 0XC0 //  HWSG upload  tem  2 times


//  枚举定义仪器类型  
#define HWSG_TYPE_HIGHTEM   0X1 //  HWSG
#define HWSG_TYPE_MIDTEM    0X2  //  HWSG
#define HWSG_TYPE_LOWTEM    0X3  //  HWSG
#define HWSG_TYPE_PortableTEM 0X4  //  HWSG
enum HWSG2C_TYPE { HWSG_TYPE_HIGHTEM, HWSG_TYPE_MIDTEM, HWSG_TYPE_LOWTEM,HWSG_TYPE_PortableTEM} ;



#define HWSG_TryCount 50

unsigned int HWSG_RxD_TRIES = 0;
bool HwSG_LookFor = true;





struct HWSG_Temp     
{
  uint16_t ObjTemp; //  目标温度
  uint16_t AmbTemp  //  环境温度
};                  //HWSG online串口数据 数值结构体

struct HWSGOnline_Uart_frame
{
  uint8_t HwSG_RX_head;   //  0xc0 接受的帧头
  uint8_t HwSG_RX_data[8]; //  8帧BYTE数据
  uint8_t RX_state     // 接受状态 0= 超时无响应  1=正常  2=异常数据
};                        //HWSGonline串口数据接收队列结构体



struct HWSG_Parameters_Str // HWSG2C 设定数据结构体  16字节
{
  uint8_t HwSG_Parameters_frame[16]; //  0xc0  原始数据
    //  数据抽象体
  uint8_t HwSGsetup0_radiant;         //  发射率坡度  9.9   -9.9    20%--20%
  uint8_t HwSGsetup1_420mARate;       //  4-20MA 微调  9.9%   -9.9%
  uint8_t HwSGsetup2_DisSperiod;      //  0.1-9.9
  uint8_t HwSGsetup3_420mAStartPoint; //  X100
  uint8_t HwSGsetup4_420mAENDtPoint;  //   X100
  uint8_t HwSGsetup5_AntiBaseLine;    //  20-40
  bool    HwSGsetup6_LockBit;         //  true or  faule
  uint8_t HwSGsetup7_UartID;          //  0-F
  uint16_t HwSGsetup8_TEMUPLimit;     //   X100
  uint16_t HwSGsetup9_TEMDOWNLimit;   //   X100
  uint8_t HwSGsetup10_GapIn1Sec;      //  Gap limit of thermometricbase in one second
  uint8_t HwSGsetup11_OverSignalline; //
  uint8_t HwSGsetup12_Backup          //
};                                    //HWSG  参数   结构体

struct tm
{
  int tm_sec;   // 秒，正常范围从 0 到 59，但允许至 61
  int tm_min;   // 分，范围从 0 到 59
  int tm_hour;  // 小时，范围从 0 到 23
  int tm_mday;  // 一月中的第几天，范围从 1 到 31
  int tm_mon;   // 月，范围从 0 到 11
  int tm_year;  // 自 1900 年起的年数
  int tm_wday;  // 一周中的第几天，范围从 0 到 6，从星期日算起
  int tm_yday;  // 一年中的第几天，范围从 0 到 365，从 1 月 1 日算起
  int tm_isdst; // 夏令时
}

class IR_Sensor_HWSG2C_Online // HWSG2C  仪器类
{
public:                                                          //  公有方法  公有变量
  IR_Sensor_HWSG2C_Online(uint8_t HWSGAddress, HWSG2C_TYPE Type, uint8_t Uartport); // 构造函数 1 仪器ID ，温度类型    strcuct function
  boolean Begin();
  boolean Set_HWSG2C_parameters(uint8_t HWSGAddress ,HWSG_setup_str Parameters_HWSG = HWSG_setup_default); // 设置参数
  HWSG_setup_str Get_HWSG2C_parameters(uint8_t HWSGAddress);                                               // 设置参数
  HWSG_T GetHWSGTemp(uint8_t HWSGAddress = 0);                                                             //default  no  is  0  // 读取温度+ 环境温度

private: //  成员变量  小写加下划线  私有方法  + 私有
  time_t  _StartUse_Date;       //  启用日期
  string  _HWSGuser_;           //  使用单位
  uint64_t  _HWSG_ESP32Mac;     //getEfuseMac() 获取eps32芯片mac地址（6byte），该地址也可以用作芯片ID；
  HWSG2C_TYPE _Machine_Type;    //  高温  中  低温
  uint8_t _Me_HwsgAddress;     //  0-15

  // uint8_t HWSGAddress; //  0-15
  // boolean HwSGsetup6_LockBit;  //  true or  faule
  uint8_t _encodedCharCount;

  boolean Transform_HWSGUART_To_Temp(HWSG_Uart_frame huf, HWSG_T ht);
  void TXD_GETTEM_Handshake(uint8_t HWSGAddress = 0); // 0-15+0xC0  连续发两次  命令送温度数据  CN
  void TXD_RESET_HWSG(uint8_t HWSGAddress = 0); // 0-15+0xF0  连续发两次  命令reset HWSG FN
  void TXD_GETpar_Handshake(uint8_t HWSGAddress = 0); // 连续发两次  命令HWSG送出工作参数  DN
  void TXD_SETpar_Handshake(uint8_t HWSGAddress = 0); // 连续发两次  命令HWSG收工作参数  EN

  void RXD_TEM_Frame(uint8_t HWSGAddress = 0);        // 发出 C0+ 后 等待接受 C0+8帧byte温度数据
  void RXD_Parameters_HWSG(uint8_t HWSGAddress = 0);  // 发出 D0+ 后 等待接受 D0+16帧byte Parameters
  void RXD_ParOK_16Parameters(uint8_t HWSGAddress = 0); // 发出 E0+ 后 接受到 E0+  正确后送 16帧byte Parameters

  uint8_t RXD_TEM_Frame(HWSG_Temp *HWSG_T ,uint8_t HWSGAddress, uint16_t timeout); //
  HWSG_Parameters_str RXD_Parameters(uint8_t HWSGAddress = 0);      //
}

/******************************************************************************
  Copyright (C) <2020>  <minguang co.ltd txy>
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.


 ******************************************************************************/