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

#include "Arduino.h"
#if defined(__AVR__) || defined(ESP8266)
#include <SoftwareSerial.h>
#elif defined(FREEDOM_E300_HIFIVE1)
#include <SoftwareSerial32.h>
#define SoftwareSerial SoftwareSerial32
#endif




#define _HWSG_VERSION "1.20.4" // software version of this library

// 握手标志  连续两次去  回一个
#define _HWSG_GETTEM_CMD0  0xC0
#define _HWSG_GETPAR_CMD0  0xD0
#define _HWSG_SETPAR_CMD0  0xE0
#define _HWSG_RESET_CMD0   0xF0
// HWSG 取温度 uart 返回状态  
#define HWSG_UART_OK 0
#define HWSG_UART_TIMEOUT 1
#define HWSG_UART_BADID 2
#define HWSG_UART_BADPACKET 3
// HWSG 取温度 TEM 返回状态
#define HWSG_TEM_OK 1
#define HWSG_TEM_illegal 0

#define HWSG2C_uart_timeout   1000       //  uart_timeout   1000 ms

#define HWSG2C_baudrate   1200       //      baudrate   1200 


//  枚举定义仪器类型  
#define HWSG_TYPE_HIGHTEM   0X1 //  HWSG
#define HWSG_TYPE_MIDTEM    0X2  //  HWSG
#define HWSG_TYPE_LOWTEM    0X3  //  HWSG
#define HWSG_TYPE_PortableTEM 0X4  //  HWSG
enum HWSG2C_TYPE { HWSG_TYPE_HIGHTEM, HWSG_TYPE_MIDTEM, HWSG_TYPE_LOWTEM,HWSG_TYPE_PortableTEM} ;

struct HWSG_Temp     
{
  uint16_t ObjTemp; //  目标温度
  uint16_t AmbTemp;  //  环境温度
  uint8_t Temp_State //   0xFF超时无响应  0x00正常 0xFE异常数据   HWSG 取温度 uart 返回状态
};                  //HWSG online串口数据 数值结构体

struct HWSGOnline_Uart_frame
{
  //  uint8_t HwSG_RX_head;   //  0xc0 接受的帧头
  uint8_t HwSG_RX_data[9]; //  帧头+ 8帧BYTE数据
  uint8_t RX_state         // 0xFF超时无响应  0x00正常 0xFE异常数据   HWSG 取温度 uart 返回状态
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
public:
   //公有变量
   uint8_t _ID_HWSG2C;     //  0-15
   uint8_t _HWSG2C_Type;   //  高温  中  低温
   time_t  _StartUse_Date;       //  启用日期
   string  _HWSG_user;           //  使用单位
   uint64_t  _HWSG_ESP32Mac;     //getEfuseMac() 获取eps32芯片mac地址（6byte），该地址也可以用作芯片ID； 
   //  公有方法
   IR_Sensor_HWSG2C_Online(uint8_t HWSGAddress, uint8_t Type, HardwareSerial *HardwareSerialport); // 构造函数 1 仪器ID ，温度类型    strcuct function
   boolean Begin(uint32_t baudrate);
   boolean Set_HWSG2C_parameters(uint8_t HWSGAddress, HWSG_Parameters_Str Parameters_HWSG = HWSG_setup_default); // 设置参数
   HWSG_Parameters_Str Get_HWSG2C_parameters(uint8_t HWSGAddress);                                          // 设置参数
   HWSG_Temp GetHWSGTemp(uint8_t HWSGAddress);                                                              //default  no  is  0  // 读取温度+ 环境温度

 private: //  成员变量  小写加下划线  私有方法  + 私有
   HWSG_Temp HWSGUART_Transto_Temp(HWSGOnline_Uart_frame huf);
   void TXD_GETTEM_Handshake(uint8_t HWSGAddress );   // 0-15+0xC0  连续发两次  命令送温度数据  CN
   void TXD_RESET_HWSG(uint8_t HWSGAddress );         // 0-15+0xF0  连续发两次  命令reset HWSG FN
   void TXD_GETpar_Handshake(uint8_t HWSGAddress );   // 连续发两次  命令HWSG送出工作参数  DN
   void TXD_SETpar_Handshake(uint8_t HWSGAddress );   // 连续发两次  命令HWSG收工作参数  EN
   HWSGOnline_Uart_frame RXD_TEM_Frame(uint8_t HWSGAddress); // 发出 C0+ 后 等待接受 C0+8帧byte温度数据
   HWSG_Parameters_Str   RXD_Parameters_HWSG(uint8_t HWSGAddress );    // 发出 D0+ 后 等待接受 D0+16帧byte Parameters
   void RXD_ParOK_16Parameters(uint8_t HWSGAddress ); // 发出 E0+ 后 接受到 E0+  正确后送 16帧byte Parameters
  
                      //

 protected:
   Stream *_H2CStream;
   HardwareSerial *h2CSerial;
}

