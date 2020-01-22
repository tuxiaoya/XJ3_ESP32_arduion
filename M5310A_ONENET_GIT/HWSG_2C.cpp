/*
  1-21
  hwsg rs232   function  
*/

#include "HWSG_2C.H"

HWSG_2C::HWSG_2C(uint8_t Addr) //   No of HWSG   
{
    DiviceAddress = Addr;     //  default  0
}

boolean HWSG_2C::begin(unsigned long baud, uint32_t config, int8_t rxPin, int8_t txPin, bool invert, unsigned long timeout_ms) //
{
    ; //  
}

uint16_t IR_Thermometer_Sensor_MLX90614::GetObjectTemp_Celsius(uint8_t SMbAddress)
{
  // setMLXAddress(SMbAddress);
  return GetTemp(MLX90614_TOBJ1); //Get celsius temperature of the object
}

