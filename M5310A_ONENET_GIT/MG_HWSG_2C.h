/*
  1-9
  hwsg rs232   function  .h   

  3-24  ������
  4-2 ����������
  4-7 GIT���ڰ�
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

// ���ֱ�־  ��������ȥ  ��һ��
#define _HWSG_GETTEM_CMD0  0xC0
#define _HWSG_GETPAR_CMD0  0xD0
#define _HWSG_SETPAR_CMD0  0xE0
#define _HWSG_RESET_CMD0   0xF0

// HWSG ȡ�¶� uart ����״̬  
#define HWSG_UART_OK 0x00
#define HWSG_UART_TIMEOUT 0xFF
#define HWSG_UART_BADPACKET 0xFE

// Define GPIO of  UART �� RXTX is ESP32 �� RXTX
#define M5310_RX3 3 //  origion lib is  rx1  tx3 ,because my faule to rhe wrong gpio  default is HardwareSerial 0
#define M5310_TX1 1 //  ע�⣬���Ź�ϵ�е��ƣ�����Ĭ�ϵ�TX0 ��Ӧ ESP32������GPIO1 ��Ӧ DEV����BOARD���ӵ�TX0���� ��ӦESP32WROOMģ���35�ſ�

#define DIS_RX9 9   //  to diwen lcd diaplay ,  default is HardwareSerial 1
#define DIS_TX10 10 //  �Դ�����

#define HWSG_RX16 16 // to  hwsg uart  ,default is HardwareSerial 2
#define HWSG_TX17 17

#define HWSG_CMD_Uploadtem0 0XC0 //  HWSG upload  tem  2 times


//  ö�ٶ�����������  
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
  uint16_t ObjTemp; //  Ŀ���¶�
  uint16_t AmbTemp  //  �����¶�
};                  //HWSG online�������� ��ֵ�ṹ��

struct HWSGOnline_Uart_frame
{
  uint8_t HwSG_RX_head;   //  0xc0 ���ܵ�֡ͷ
  uint8_t HwSG_RX_data[8]; //  8֡BYTE����
  uint8_t RX_state     // ����״̬ 0= ��ʱ����Ӧ  1=����  2=�쳣����
};                        //HWSGonline�������ݽ��ն��нṹ��



struct HWSG_Parameters_Str // HWSG2C �趨���ݽṹ��  16�ֽ�
{
  uint8_t HwSG_Parameters_frame[16]; //  0xc0  ԭʼ����
    //  ���ݳ�����
  uint8_t HwSGsetup0_radiant;         //  �������¶�  9.9   -9.9    20%--20%
  uint8_t HwSGsetup1_420mARate;       //  4-20MA ΢��  9.9%   -9.9%
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

class IR_Sensor_HWSG2C_Online // HWSG2C  ������
{
public:                                                          //  ���з���  ���б���
  IR_Sensor_HWSG2C_Online(uint8_t HWSGAddress, HWSG2C_TYPE Type, uint8_t Uartport); // ���캯�� 1 ����ID ���¶�����    strcuct function
  boolean Begin();
  boolean Set_HWSG2C_parameters(uint8_t HWSGAddress ,HWSG_setup_str Parameters_HWSG = HWSG_setup_default); // ���ò���
  HWSG_setup_str Get_HWSG2C_parameters(uint8_t HWSGAddress);                                               // ���ò���
  HWSG_T GetHWSGTemp(uint8_t HWSGAddress = 0);                                                             //default  no  is  0  // ��ȡ�¶�+ �����¶�

private: //  ��Ա����  Сд���»���  ˽�з���  + ˽��
  time_t  _StartUse_Date;       //  ��������
  string  _HWSGuser_;           //  ʹ�õ�λ
  uint64_t  _HWSG_ESP32Mac;     //getEfuseMac() ��ȡeps32оƬmac��ַ��6byte�����õ�ַҲ��������оƬID��
  HWSG2C_TYPE _Machine_Type;    //  ����  ��  ����
  uint8_t _Me_HwsgAddress;     //  0-15

  // uint8_t HWSGAddress; //  0-15
  // boolean HwSGsetup6_LockBit;  //  true or  faule
  uint8_t _encodedCharCount;

  boolean Transform_HWSGUART_To_Temp(HWSG_Uart_frame huf, HWSG_T ht);
  void TXD_GETTEM_Handshake(uint8_t HWSGAddress = 0); // 0-15+0xC0  ����������  �������¶�����  CN
  void TXD_RESET_HWSG(uint8_t HWSGAddress = 0); // 0-15+0xF0  ����������  ����reset HWSG FN
  void TXD_GETpar_Handshake(uint8_t HWSGAddress = 0); // ����������  ����HWSG�ͳ���������  DN
  void TXD_SETpar_Handshake(uint8_t HWSGAddress = 0); // ����������  ����HWSG�չ�������  EN

  void RXD_TEM_Frame(uint8_t HWSGAddress = 0);        // ���� C0+ �� �ȴ����� C0+8֡byte�¶�����
  void RXD_Parameters_HWSG(uint8_t HWSGAddress = 0);  // ���� D0+ �� �ȴ����� D0+16֡byte Parameters
  void RXD_ParOK_16Parameters(uint8_t HWSGAddress = 0); // ���� E0+ �� ���ܵ� E0+  ��ȷ���� 16֡byte Parameters

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