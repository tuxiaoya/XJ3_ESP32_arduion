/*
  1-9
  hwsg rs232   function  .h   
*/

#ifndef HWSG_2C_H
#define HWSG_2C_H

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

// Define GPIO of  UART ： RXTX is ESP32 的 RXTX  
#define M5310_RX3  3      //  origion lib is  rx1  tx3 ,because my faule to rhe wrong gpio  default is HardwareSerial 0
#define M5310_TX1  1      //  注意，引脚关系有点绕：驱动默认的TX0 对应 ESP32的引脚GPIO1 对应 DEV——BOARD板子的TX0插针 对应ESP32WROOM模组的35号孔

#define DIS_RX9   9       //  to diwen lcd diaplay ,  default is HardwareSerial 1
#define DIS_TX10  10      //  以此类推   

#define HWSG_RX16 16      // to  hwsg uart  ,default is HardwareSerial 2
#define HWSG_TX17 17




#define HWSG_CMD_Uploadtem0 0XC0  //  HWSG upload  tem  2 times

typedef struct  HWSG_Uart_frame
{
  uint8_t  HwSG_RX_head;   //  0xc0 接受的帧头
  uint8_t  HwSG_RX_data[8] //  8帧BYTE数据
} ; //HWSG串口数据接收队列结构体

typedef struct  HWSG_T
{
  uint16_t  ObjTemp;   //  目标温度 
  uint16_t  AmbTemp    //  环境温度
} ; //HWSG串口数据接收队列结构体


typedef struct HWSG_setup_str
{
  uint8_t HwSG_setup_frame[16]; //  0xc0
  
  uint8_t HwSGsetup0_radiant; //  发射率坡度  9.9   -9.9    20%--20%
  uint8_t HwSGsetup1_420mARate;  //  4-20MA 微调  9.9%   -9.9%
  uint8_t HwSGsetup2_DisSperiod; //  0.1-9.9
} ; //HWSG  参数   结构体



class HWSG_2C
{
public:
  boolean SetHWSGTemp(HWSG_setup_str set_HWSG = HWSG_setup_default);
  HWSG_T  GetHWSGTemp(uint8_t No_HWSG = 0);  //default  no  is  0

private:
  boolean Transform_HWSGUART_to_Temp(HWSG_Uart_frame huf,  HWSG_T ht);
};
/******************************************************************************
  Copyright (C) <2020>  <txy>
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.


 ******************************************************************************/