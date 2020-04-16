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

#include "Arduino.h"
#if defined(__AVR__) || defined(ESP8266)
#include <SoftwareSerial.h>
#elif defined(FREEDOM_E300_HIFIVE1)
#include <SoftwareSerial32.h>
#define SoftwareSerial SoftwareSerial32
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

#define HWSG2C_baudrate   1200       //  HWSG upload  tem  2 times

//  ö�ٶ�����������  
#define HWSG_TYPE_HIGHTEM   0X1 //  HWSG
#define HWSG_TYPE_MIDTEM    0X2  //  HWSG
#define HWSG_TYPE_LOWTEM    0X3  //  HWSG
#define HWSG_TYPE_PortableTEM 0X4  //  HWSG
enum HWSG2C_TYPE { HWSG_TYPE_HIGHTEM, HWSG_TYPE_MIDTEM, HWSG_TYPE_LOWTEM,HWSG_TYPE_PortableTEM} ;

struct HWSG_Temp     
{
  uint16_t ObjTemp; //  Ŀ���¶�
  uint16_t AmbTemp;  //  �����¶�
  uint8_t Temp_State //   0xFF��ʱ����Ӧ  0x00���� 0xFE�쳣����   HWSG ȡ�¶� uart ����״̬
};                  //HWSG online�������� ��ֵ�ṹ��

struct HWSGOnline_Uart_frame
{
  uint8_t HwSG_RX_head;   //  0xc0 ���ܵ�֡ͷ
  uint8_t HwSG_RX_data[8]; //  8֡BYTE����
  uint8_t RX_state         // 0xFF��ʱ����Ӧ  0x00���� 0xFE�쳣����   HWSG ȡ�¶� uart ����״̬
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
public:
   //���б���
   uint8_t _ID_HWSG2C;     //  0-15
   uint8_t _HWSG2C_Type;   //  ����  ��  ����
   time_t  _StartUse_Date;       //  ��������
   string  _HWSG_user;           //  ʹ�õ�λ
   uint64_t  _HWSG_ESP32Mac;     //getEfuseMac() ��ȡeps32оƬmac��ַ��6byte�����õ�ַҲ��������оƬID�� 
   //  ���з���
   IR_Sensor_HWSG2C_Online(uint8_t HWSGAddress, uint8_t Type, HardwareSerial *HardwareSerialport); // ���캯�� 1 ����ID ���¶�����    strcuct function
   boolean Begin(uint32_t baudrate);
   boolean Set_HWSG2C_parameters(uint8_t HWSGAddress, HWSG_Parameters_Str Parameters_HWSG = HWSG_setup_default); // ���ò���
   HWSG_Parameters_Str Get_HWSG2C_parameters(uint8_t HWSGAddress);                                          // ���ò���
   HWSG_Temp GetHWSGTemp(uint8_t HWSGAddress);                                                              //default  no  is  0  // ��ȡ�¶�+ �����¶�

 private: //  ��Ա����  Сд���»���  ˽�з���  + ˽��
   boolean HWSGUART_Transto_Temp(HWSGOnline_Uart_frame huf, HWSG_Temp ht);
   void TXD_GETTEM_Handshake(uint8_t HWSGAddress );   // 0-15+0xC0  ����������  �������¶�����  CN
   void TXD_RESET_HWSG(uint8_t HWSGAddress );         // 0-15+0xF0  ����������  ����reset HWSG FN
   void TXD_GETpar_Handshake(uint8_t HWSGAddress );   // ����������  ����HWSG�ͳ���������  DN
   void TXD_SETpar_Handshake(uint8_t HWSGAddress );   // ����������  ����HWSG�չ�������  EN
   HWSGOnline_Uart_frame RXD_TEM_Frame(uint8_t HWSGAddress); // ���� C0+ �� �ȴ����� C0+8֡byte�¶�����
   void RXD_Parameters_HWSG(uint8_t HWSGAddress );    // ���� D0+ �� �ȴ����� D0+16֡byte Parameters
   void RXD_ParOK_16Parameters(uint8_t HWSGAddress ); // ���� E0+ �� ���ܵ� E0+  ��ȷ���� 16֡byte Parameters

   uint8_t RXD_TEM_Frame(HWSG_Temp *HWSG_T, uint8_t HWSGAddress, uint16_t timeout); //
   HWSG_Parameters_Str RXD_Parameters(uint8_t HWSGAddress = 0);                     //

 protected:
   Stream *_H2CStream;
   HardwareSerial *h2CSerial;
}

