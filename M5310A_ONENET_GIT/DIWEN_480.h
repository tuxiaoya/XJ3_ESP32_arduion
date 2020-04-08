/*
  DIWEN_480.h - DIWEN 480 UART_LCD library for Arduino

*/

#define dpx0 0
#define dpx1 400        //
#define dpx2 800        //rrrrrggggggbbbbb
#define dc_white 0xffff //         B1111111111111111
#define dc_red 0xf800   //    B1111100000000000
#define dc_gre 0x07e0   //    B0000011111100000
#define dc_blue 0x001f  // B0000000000011111



class DIWEN480     //  diwen uart 800X480 lcd class
{ public:
    DIWEN480();
    void Begin();
    void DisMessage(char No_place, String msg, uint16_t dcc) ;
    void DisStrings(uint8_t smode, uint16_t cc, uint16_t dc, uint16_t x, uint16_t y, String dstr);
    void SetFace(uint8_t face);
    void SetLightness(uint8_t Light);
    void LightIn(uint8_t lspeed);
    void LightOut(uint8_t lspeed);


  private:

  

  
}
