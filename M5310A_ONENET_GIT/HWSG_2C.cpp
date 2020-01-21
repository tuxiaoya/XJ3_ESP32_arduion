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

