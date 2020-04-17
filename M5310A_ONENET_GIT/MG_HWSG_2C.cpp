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



HWSG_Temp IR_Sensor_HWSG2C_Online::GetHWSGTemp(uint8_t HWSGAddress)
  { HWSGOnline_Uart_frame  Huf;
    TXD_GETTEM_Handshake(HWSGAddress);    //����������  0-15+0xC0 
    Huf=RXD_TEM_Frame(HWSGAddress);      // �ȴ�����uart����֡  �� huf
    return HWSGUART_Transto_Temp(Huf);   // ������֡ת��Ϊ �¶�+�����¶�+����״̬  ����
  }

  // #define HWSG_TEM_OK 1
  // #define HWSG_TEM_illegal 0
  // ������֡ת��Ϊ �¶�+�����¶�+����״̬
  HWSG_Temp IR_Sensor_HWSG2C_Online::HWSGUART_Transto_Temp(HWSGOnline_Uart_frame huf)
  {
    HWSG_Temp ht;
    uint16_t  h0,h1,h2,h3;   // ho is  low byte of frame 
    h0 = huf.HwSG_RX_data[1] >> 4; 
    h1 = huf.HwSG_RX_data[2] >> 4;h1=h1<<4;
    h2 = huf.HwSG_RX_data[3] >> 4;
    h2 = h2 << 8; // h2 must be <8 else tem will over 2048 thatis illegal
    if (h2 >= 8)
    { // otherwise the tem data is illegal
      ht.Temp_State = HWSG_TEM_illegal;
      return ht;
    }
    h3 = huf.HwSG_RX_data[4] >> 4;  // h3 must be 0 
    if (h3!=0) {     // otherwise the tem data is illegal
      ht.Temp_State = HWSG_TEM_illegal;
      return ht;
    }
      ht.ObjTemp = h0 + h1 + h2 + h3;  // ���Ŀ���¶�
      // �����¶�
      h0 = huf.HwSG_RX_data[7] >> 4;
      h1 = huf.HwSG_RX_data[8] >> 4; h1 = h1 << 4;
      // h1 must be <7  else �����¶� will over 111 ��  that is impossible
      if (h1 >= 7)  // otherwise the tem data is illegal
      { 
        ht.Temp_State = HWSG_TEM_illegal;
        return ht;
      }
      // noa ,the  tem is ok 
      ht.AmbTemp=h0+h1;
      ht.Temp_State = HWSG_TEM_OK;
      return ht;
  }

      
      //  �������¶�����  CN   ��������
void IR_Sensor_HWSG2C_Online::TXD_GETTEM_Handshake(uint8_t HWSGAddress) //  ����������  0-15+0xC0  ����������  �������¶�����  CN
  {
    SERIAL_WRITE(HWSGAddress + _HWSG_GETTEM_CMD0); // send 0xc0+ 2 times
    SERIAL_WRITE(HWSGAddress + _HWSG_GETTEM_CMD0); // send 0xc0+ 2 times
  }

// ��λ HWSG2  �޷���
  void IR_Sensor_HWSG2C_Online::TXD_RESET_HWSG(uint8_t HWSGAddress) // 
  {
    SERIAL_WRITE(HWSGAddress + _HWSG_RESET_CMD0); 
    SERIAL_WRITE(HWSGAddress + _HWSG_RESET_CMD0); 
  }

// ����HWSG�ͳ���������  DN
  void IR_Sensor_HWSG2C_Online::TXD_GETpar_Handshake(uint8_t HWSGAddress) 
  {
    SERIAL_WRITE(HWSGAddress + _HWSG_GETPAR_CMD0); // send 0xD0+0 2 times  to  rev  parameters from  HWSG2C
    SERIAL_WRITE(HWSGAddress + _HWSG_GETPAR_CMD0);
  }

// ����HWSG�չ�������  
void IR_Sensor_HWSG2C_Online::TXD_SETpar_Handshake(uint8_t HWSGAddress) // ����HWSG�չ�������  DN
{
  SERIAL_WRITE(HWSGAddress + _HWSG_SETPAR_CMD0); // send 0xE0+0 2 times  to   send parameters to HWSG2C
  SERIAL_WRITE(HWSGAddress + _HWSG_SETPAR_CMD0);
}

//  URAT ���ܴ������    ��ȷ9֡
/**************************************************************************/
/*!
    @brief   to receive data over UART from the HWSG2c and process it into a packet
    @param   tuint8_t HWSGAddress
    @returns HWSGOnline_Uart_frame
    @returns <code>FINGERPRINT_TIMEOUT</code> or <code>FINGERPRINT_BADPACKET</code> on failure
*/
/**************************************************************************/
HWSGOnline_Uart_frame IR_Sensor_HWSG2C_Online::RXD_TEM_Frame(uint8_t HWSGAddress) // ���� C0+ �� �ȴ����� C0+8֡byte�¶�����
{  
  uint8_t inByte;
  uint8_t idx = 0;
  unsigned long currentMillis = millis();
  unsigned long TxDstart_Millis;
  HWSGOnline_Uart_frame reading_frame;  
  TxDstart_Millis = currentMillis;
  while (currentMillis - TxDstart_Millis <  HWSG2C_uart_timeout) //  �ж�UART �Ƿ���ܳ�ʱ 
  { currentMillis = millis();   //     
    if ( h2CSerial->available())
    {
    inByte = h2CSerial->read();// get incoming byte:     
    reading_frame.HwSG_RX_data[idx]=inByte;
    if(idx==0)  
    {
      if(reading_frame.HwSG_RX_data[0]!= HWSGAddress)  // ֡ͷ����ͬ �߼�����
        {reading_frame.RX_state=HWSG_UART_BADID ;  
         return reading_frame;}      
    }    
    else if( (reading_frame.HwSG_RX_data[idx]>>4)!=idx)  // ����4λ λ���������ݱ�����
    {
     reading_frame.RX_state=HWSG_UART_BADPACKET ; // ֡���߼�����
     return reading_frame; 
    }
    else if(idx==8)  {
    reading_frame.RX_state=HWSG_UART_OK ;   // ֡�������� 
    return reading_frame;
    } 
    inByte++;
    }
  }
// ֡���ݳ�ʱ
reading_frame.RX_state=HWSG_UART_TIMEOUT ;  
return reading_frame;

}


HWSG_Parameters_Str IR_Sensor_HWSG2C_Online::RXD_Parameters_HWSG(uint8_t HWSGAddress = 0) // ���� D0+ �� �ȴ����� D0+16֡byte Parameters
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