/*
  1-21
  hwsg rs232   function  

*/

#include "MG_HWSG_2C.H"

IR_Sensor_HWSG2C_Online::IR_Sensor_HWSG2C_Online(uint8_t HWSGAddress , HWSG2C_TYPE Type)
{
   _Machine_Type=HWSGAddress;    //  高温  中  低温
  _Me_HwsgAddress=Type; 
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
bool IR_Sensor_HWSG2C_Online::RXD_TEM_Frame(uint8_t HWSGAddress = 0) // 发出 C0+ 后 等待接受 C0+8帧byte温度数据
{
  HwSG_LookFor = true;

  HWSG_RxD_TRIES = 0;
  while (HwSG_LookFor)
  {
    while (HWSG_Serial.available() > 0)
    {

      HWSG_RxD_TRIES++;
      // get incoming byte:
      inByte = HWSG_Serial.read();
    }

   // if too many time for get data force exit   #define HWSG_TryCount 50
   if (HWSG_RxD_TRIES >  HWSG_TryCount)
   {
#ifdef DEBUG
     Serial.println("Gps end for max tries");
#endif
     g_dev_gpsLookFor = false;
   }

}
}

bool IR_Sensor_HWSG2C_Online::encode(char c)
{
  ++encodedCharCount;  // maybe wo need a  terminators  thinkabout to add  in BWS_ASM  

  switch (encodedCharCount)   //  case  every byte  0-7
  {
  case 0 : //   
  if (c== ) 
  default: // ordinary characters
    if (curTermOffset < sizeof(term) - 1)
      term[curTermOffset++] = c;
    if (!isChecksumTerm)
      parity ^= c;
    return false;
  }

  return false;
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