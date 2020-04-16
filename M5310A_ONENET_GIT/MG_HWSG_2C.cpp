/*
  1-21
  hwsg rs232   function  
  4-3 ʹ��millis()����
  4-16 HardwareSerial *HardwareSerialport     use point class transform 
*/

#include "MG_HWSG_2C.H"

#define SERIAL_WRITE(...) h2CSerial->write(__VA_ARGS__)

IR_Sensor_HWSG2C_Online::IR_Sensor_HWSG2C_Online(uint8_t HWSGAID, uint8_t Type, HardwareSerial *HardwareSerialport)
{
  _ID_HWSG2C = HWSGAID;
  _HWSG2C_Type=Type; //  ����  ��  ����

  h2CSerial = HardwareSerialport;
  H2CStream = h2CSerial;
}

boolean IR_Sensor_HWSG2C_Online::Begin(uint32_t baudrate)
{
  if (h2CSerial)  h2CSerial->begin(baudrate);
}

boolean IR_Sensor_HWSG2C_Online::HWSGUART_Transto_Temp(HWSGOnline_Uart_frame huf, HWSG_Temp ht)
{   
  }

HWSG_Temp IR_Sensor_HWSG2C_Online::GetHWSGTemp(uint8_t HWSGAddress)
  {
    TXD_GETTEM_Handshake(HWSGAddress);


  }
  //  ��������  
  void IR_Sensor_HWSG2C_Online::TXD_GETTEM_Handshake(uint8_t HWSGAddress)  //  ����������  0-15+0xC0  ����������  �������¶�����  CN
  {
    SERIAL_WRITE(HWSGAddress + _HWSG_GETTEM_CMD0); // send 0xc0+ 2 times
    SERIAL_WRITE(HWSGAddress + _HWSG_GETTEM_CMD0); // send 0xc0+ 2 times
  }

  void IR_Sensor_HWSG2C_Online::TXD_RESET_HWSG(uint8_t HWSGAddress) // 
  {
    SERIAL_WRITE(HWSGAddress + _HWSG_RESET_CMD0); // send 0xc0+ 2 times
  }

  void IR_Sensor_HWSG2C_Online::TXD_GETpar_Handshake(uint8_t HWSGAddress) // ����HWSG�ͳ���������  DN
  {
    SERIAL_WRITE(HWSGAddress + _HWSG_GETPAR_CMD0); // send 0xD0+0 2 times  to  rev  parameters from  HWSG2C
    SERIAL_WRITE(HWSGAddress + _HWSG_GETPAR_CMD0);
  }

void IR_Sensor_HWSG2C_Online::TXD_SETpar_Handshake(uint8_t HWSGAddress) // ����HWSG�չ�������  DN
{
  SERIAL_WRITE(HWSGAddress + _HWSG_SETPAR_CMD0); // send 0xE0+0 2 times  to   send parameters to HWSG2C
  SERIAL_WRITE(HWSGAddress + _HWSG_SETPAR_CMD0);
}

//  URAT ���ܴ������    ��ȷ9֡
/**************************************************************************/
/*!
    @brief   to receive data over UART from the HWSG2c and process it into a packet
    @param   packet A structure containing the bytes received
    @param   timeout how many milliseconds we're willing to wait
    @returns <code>FINGERPRINT_OK</code> on success
    @returns <code>FINGERPRINT_TIMEOUT</code> or <code>FINGERPRINT_BADPACKET</code> on failure
*/
/**************************************************************************/
const long Wait_HWSG_period = 1000;  //   RXD_TEM_Frame(HWSG_Temp *HWSG_T, uint8_t HWSGAddress, uint16_t timeout);                                                                      // period at which to blink in ms
HWSGOnline_Uart_frame IR_Sensor_HWSG2C_Online::RXD_TEM_Frame(uint8_t HWSGAddress) // ���� C0+ �� �ȴ����� C0+8֡byte�¶�����
{  
  uint8_t inByte;
  uint16_t idx = 0, timer = 0;
  unsigned long currentMillis = millis();
  unsigned long TxDedC0C0_Millis;
  // unsigned long period = 1000;

  TxDedC0C0_Millis = currentMillis;

  while (currentMillis - TxDedC0C0_Millis <=  timeout) //  �ж�UART �Ƿ���ܳ�ʱ 
  { currentMillis = millis();   //    delay(1);     timer++;
    while (!HWSG_Serial.available() )
    {      
      if (timer >= timeout)
      {
        #ifdef FINGERPRINT_DEBUG
        Serial.println("Timed out");
        #endif
        
      }
    }
    inByte = HWSG_Serial.read();// get incoming byte:
    }
    return HWSG_UART_TIMEOUT;
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