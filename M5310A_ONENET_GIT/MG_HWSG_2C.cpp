/*
  1-21
  hwsg rs232   function  

*/

#include "MG_HWSG_2C.H"

IR_Sensor_HWSG2C_Online::IR_Sensor_HWSG2C_Online(uint8_t HWSGAddress , HWSG2C_TYPE Type)
{
   _Machine_Type=HWSGAddress;    //  ����  ��  ����
  _Me_HwsgAddress=Type; 
}

boolean IR_Sensor_HWSG2C_Online::begin(uint8_t HWSGAddress)
{
  GetHWSGTemp(uint8_t HWSGAddress);
}

 boolean IR_Sensor_HWSG2C_Online::Transform_HWSGUART_To_Temp(HWSG_Uart_frame huf, HWSG_T ht)
  {
   
  }
//  ��������  
  void IR_Sensor_HWSG2C_Online::TXD_GETTEM_Handshake(uint8_t HWSGAddress = 0)  //  ����������  0-15+0xC0  ����������  �������¶�����  CN
  {   HWSG_Serial.write(HWSGAddress+0xc0);  // send 0xc0+ 2 times
      HWSG_Serial.write(HWSGAddress+0xc0);  
  }

  void IR_Sensor_HWSG2C_Online::TXD_RESET_HWSG(uint8_t HWSGAddress = 0) // 
  {   HWSG_Serial.write(HWSGAddress+0xF0);  // send 0xF0+0 2 times  to reset HWSG2C
      HWSG_Serial.write(HWSGAddress+0xF0);  
  }

  void IR_Sensor_HWSG2C_Online::TXD_GETpar_Handshake(uint8_t HWSGAddress = 0) // ����HWSG�ͳ���������  DN
  {   HWSG_Serial.write(HWSGAddress+0xD0);  // send 0xD0+0 2 times  to  rev  parameters from  HWSG2C
      HWSG_Serial.write(HWSGAddress+0xD0); 
  }

void IR_Sensor_HWSG2C_Online::TXD_SETpar_Handshake(uint8_t HWSGAddress = 0) // ����HWSG�չ�������  DN
{     HWSG_Serial.write(HWSGAddress+0xE0);  // send 0xE0+0 2 times  to   send parameters to HWSG2C
      HWSG_Serial.write(HWSGAddress+0xE0); 
}

//  ���ܴ������
/**************************************************************************/
/*!
    @brief   Helper function to receive data over UART from the sensor and process it into a packet
    @param   packet A structure containing the bytes received
    @param   timeout how many milliseconds we're willing to wait
    @returns <code>FINGERPRINT_OK</code> on success
    @returns <code>FINGERPRINT_TIMEOUT</code> or <code>FINGERPRINT_BADPACKET</code> on failure
*/
/**************************************************************************/
uint8_t IR_Sensor_HWSG2C_Online::RXD_TEM_Frame(uint8_t HWSGAddress, uint16_t timeout) // ���� C0+ �� �ȴ����� C0+8֡byte�¶�����
{
  HwSG_LookFor = true;
  uint8_t inByte;
  uint16_t idx = 0, timer = 0;
  HWSG_RxD_TRIES = 0;

  while (HwSG_LookFor)
  {
    while (!HWSG_Serial.available() )
    {
      delay(1);
      timer++;
     if (timer >= timeout)
      {
        #ifdef FINGERPRINT_DEBUG
        Serial.println("Timed out");
        #endif
        return FINGERPRINT_TIMEOUT;
      }
    }
    inByte = HWSG_Serial.read();// get incoming byte:
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

boolean IR_Sensor_HWSG2C_Online::RXD_Parameters_HWSG(uint8_t HWSGAddress = 0) // ���� D0+ �� �ȴ����� D0+16֡byte Parameters
{

}
boolean IR_Sensor_HWSG2C_Online::RXD_ParOK_16Parameters(uint8_t HWSGAddress = 0) // ���� E0+ �� ���ܵ� E0+  ��ȷ���� 16֡byte Parameters
{

}



HWSGOnline_Uart_frame IR_Sensor_HWSG2C_Online::RXD__TemDataFrame(uint8_t HWSGAddress = 0) //  first is FeeddBack  HWSGAddress
{

}

HWSG_Parameters_str IR_Sensor_HWSG2C_Online::RXD_Parameters(uint8_t HWSGAddress = 0) //  first is FeeddBack  HWSGAddress  16 frame  BYTE 
{

}