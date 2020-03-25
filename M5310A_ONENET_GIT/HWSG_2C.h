/*
  1-9
  hwsg rs232   function  .h   
*/

#ifndef HWSG_2C_H
#define HWSG_2C_H

#include <ctime>

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"

#endif

// Define GPIO of  UART �� RXTX is ESP32 �� RXTX
#define M5310_RX3 3 //  origion lib is  rx1  tx3 ,because my faule to rhe wrong gpio  default is HardwareSerial 0
#define M5310_TX1 1 //  ע�⣬���Ź�ϵ�е��ƣ�����Ĭ�ϵ�TX0 ��Ӧ ESP32������GPIO1 ��Ӧ DEV����BOARD���ӵ�TX0���� ��ӦESP32WROOMģ���35�ſ�

#define DIS_RX9 9   //  to diwen lcd diaplay ,  default is HardwareSerial 1
#define DIS_TX10 10 //  �Դ�����

#define HWSG_RX16 16 // to  hwsg uart  ,default is HardwareSerial 2
#define HWSG_TX17 17

#define HWSG_CMD_Uploadtem0 0XC0 //  HWSG upload  tem  2 times

typedef struct HWSGOnline_Uart_frame
{
  uint8_t HwSG_RX_head;   //  0xc0 ���ܵ�֡ͷ
  uint8_t HwSG_RX_data[8] //  8֡BYTE����
};                        //HWSGonline�������ݽ��ն��нṹ��

typedef struct HWSG_T
{
  uint16_t ObjTemp; //  Ŀ���¶�
  uint16_t AmbTemp  //  �����¶�
};                  //HWSG online�������� ��ֵ�ṹ��

typedef struct HWSG_setup_str // HWSG2C �趨���ݽṹ��  16�ֽ�
{
  uint8_t HwSG_setup_frame[16]; //  0xc0  ԭʼ����

  //  ���ݳ�����
  uint8_t HwSGsetup0_radiant;         //  �������¶�  9.9   -9.9    20%--20%
  uint8_t HwSGsetup1_420mARate;       //  4-20MA ΢��  9.9%   -9.9%
  uint8_t HwSGsetup2_DisSperiod;      //  0.1-9.9
  uint8_t HwSGsetup3_420mAStartPoint; //  X100
  uint8_t HwSGsetup4_420mAENDtPoint;  //   X100
  uint8_t HwSGsetup5_AntiBaseLine;    //  20-40
  boolean HwSGsetup6_LockBit;         //  true or  faule
  uint8_t HwSGsetup7_UartID;          //  0-F
  uint16_t HwSGsetup8_TEMUPLimit;     //   X100
  uint16_t HwSGsetup9_TEMDOWNLimit;   //   X100
  uint8_t HwSGsetup10_GapIn1Sec;      //  Gap limit of thermometricbase in one second
  uint8_t HwSGsetup11_OverSignalline; //
  uint8_t HwSGsetup12_Backup          //
};                                    //HWSG  ����   �ṹ��

struct tm
{
  int tm_sec;   // �룬������Χ�� 0 �� 59���������� 61
  int tm_min;   // �֣���Χ�� 0 �� 59
  int tm_hour;  // Сʱ����Χ�� 0 �� 23
  int tm_mday;  // һ���еĵڼ��죬��Χ�� 1 �� 31
  int tm_mon;   // �£���Χ�� 0 �� 11
  int tm_year;  // �� 1900 ���������
  int tm_wday;  // һ���еĵڼ��죬��Χ�� 0 �� 6��������������
  int tm_yday;  // һ���еĵڼ��죬��Χ�� 0 �� 365���� 1 �� 1 ������
  int tm_isdst; // ����ʱ
}

class HWSG_2C_Online // HWSG2C  ������
{
public:                                                          //  ���з���  ���б���
  boolean SetHWSG(HWSG_setup_str set_HWSG = HWSG_setup_default); // ���ò���
  HWSG_T GetHWSGTemp(uint8_t No_HWSG = 0);                       //default  no  is  0  // ��ȡ�¶�+ �����¶�

private: //  ��Ա����  Сд���»���  ˽�з���  + ˽��
  time_t  StartUse_Date;
  string  HWSGuser_; 
  boolean Transform_HWSGUART_to_Temp(HWSG_Uart_frame huf, HWSG_T ht);
};
/******************************************************************************
  Copyright (C) <2020>  <txy>
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.


 ******************************************************************************/