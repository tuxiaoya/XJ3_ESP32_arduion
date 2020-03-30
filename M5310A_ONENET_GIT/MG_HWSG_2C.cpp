/*
  1-21
  hwsg rs232   function  

*/

#include "MG_HWSG_2C.H"

IR_Sensor_HWSG2C_Online::IR_Sensor_HWSG2C_Online(uint8_t HWSGAddress)
{
  ThisHWSGAddress = HWSGAddress;
}

boolean IR_Sensor_HWSG2C_Online::begin(uint8_t HWSGAddress)
{
  GetHWSGTemp(uint8_t HWSGAddress);
}

 boolean IR_Sensor_HWSG2C_Online::Transform_HWSGUART_To_Temp(HWSG_Uart_frame huf, HWSG_T ht)
  {
   
  }
//  发送握手  
  void IR_Sensor_HWSG2C_Online::TXD_GETTEM_Handshake(uint8_t HWSGAddress = 0)  //  发两个握手  0-15+0xC0  连续发两次  命令送温度数据  CN
  {   HWSG_Serial.write(HWSGAddress+0xc0);  // send 0xc0+ 2 times
      HWSG_Serial.write(HWSGAddress+0xc0);  
  }

  void IR_Sensor_HWSG2C_Online::TXD_RESET_HWSG(uint8_t HWSGAddress = 0) // 
  {   HWSG_Serial.write(HWSGAddress+0xF0);  // send 0xF0+0 2 times  to reset HWSG2C
      HWSG_Serial.write(HWSGAddress+0xF0);  
  }

  void IR_Sensor_HWSG2C_Online::TXD_GETpar_Handshake(uint8_t HWSGAddress = 0) // 命令HWSG送出工作参数  DN
  {   HWSG_Serial.write(HWSGAddress+0xD0);  // send 0xD0+0 2 times  to  rev  parameters from  HWSG2C
      HWSG_Serial.write(HWSGAddress+0xD0); 
  }

void IR_Sensor_HWSG2C_Online::TXD_SETpar_Handshake(uint8_t HWSGAddress = 0) // 命令HWSG收工作参数  DN
{     HWSG_Serial.write(HWSGAddress+0xE0);  // send 0xE0+0 2 times  to   send parameters to HWSG2C
      HWSG_Serial.write(HWSGAddress+0xE0); 
}

//  接受处理程序
boolean IR_Sensor_HWSG2C_Online::RXD_TEM_Frame(uint8_t HWSGAddress = 0) // 发出 C0+ 后 等待接受 C0+8帧byte温度数据
{
  if (HWSG_Serial(2).available() > 0)
  {
    // get incoming byte:
    inByte = HWSG_Serial.read();
   
  }
}
boolean IR_Sensor_HWSG2C_Online::RXD_Parameters_HWSG(uint8_t HWSGAddress = 0) // 发出 D0+ 后 等待接受 D0+16帧byte Parameters
{

}
boolean IR_Sensor_HWSG2C_Online::RXD_ParOK_16Parameters(uint8_t HWSGAddress = 0) // 发出 E0+ 后 接受到 E0+  正确后送 16帧byte Parameters
{

}



HWSGOnline_Uart_frame IR_Sensor_HWSG2C_Online::RXD__TemDataFrame(uint8_t HWSGAddress = 0) //  first is FeeddBack  HWSGAddress
{

}

HWSG_Parameters_str IR_Sensor_HWSG2C_Online::RXD_Parameters(uint8_t HWSGAddress = 0) //  first is FeeddBack  HWSGAddress  16 frame  BYTE 
{

}