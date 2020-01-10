/*
  1-9
  hwsg rs232   function  .h   
*/

#define HWSG_CMD_Uploadtem0 0XC0  //  HWSG upload  tem  2 times

typedef struct  HWSG_Uart_frame
{
  uint8_t  HwSG_RX_head;   //  0xc0
  uint8_t  HwSG_RX_data[8]
} HWSG_Uart0; //HWSG串口数据接收队列结构体

typedef struct HWSG_setup_str
{
  uint8_t HwSG_setup_frame[16]; //  0xc0

  uint8_t HwSGsetup0_radiant; //  发射率坡度  9.9   -9.9    20%--20%
  uint8_t HwSGsetup1_420mARate;  //  4-20MA 微调  9.9%   -9.9%
  uint8_t HwSGsetup2_DisSperiod; //  0.1-9.9






} HWSG_setup; //HWSG  参数   结构体



class IR_Thermometer_DUBLECOLOR_HWSG2C
{
public:
  boolean SetHWSGTemp(uint8_t Addr_HWSG = HWSG_CMD_Uploadtem0);
  uint16_t GetHWSGTemp(uint8_t Addr_HWSG = HWSG_CMD_Uploadtem0);

private:
  float GetTemp(uint8_t Register);

  uint8_t DiviceAddress;
  void setMLXAddress(uint8_t addr);
  uint16_t Get_Register(uint8_t Reg);
  void write16(uint8_t addr, uint16_t data);
};
/******************************************************************************
  Copyright (C) <2020>  <txy>
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.


 ******************************************************************************/