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
boolean IR_Sensor_HWSG2C_Online::RXD_TEM_Frame(uint8_t HWSGAddress = 0) // ���� C0+ �� �ȴ����� C0+8֡byte�¶�����
{
  if (HWSG_Serial(2).available() > 0)
  {
    // get incoming byte:
    inByte = Serial.read();
    // read first analog input, divide by 4 to make the range 0-255:
    firstSensor = analogRead(A0) / 4;
    // delay 10ms to let the ADC recover:
    delay(10);
    // read second analog input, divide by 4 to make the range 0-255:
    secondSensor = analogRead(1) / 4;
    // read switch, map it to 0 or 255L
    thirdSensor = map(digitalRead(2), 0, 1, 0, 255);
    // send sensor values:
    Serial.write(firstSensor);
    Serial.write(secondSensor);
    Serial.write(thirdSensor);
  }
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