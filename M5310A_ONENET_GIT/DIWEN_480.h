/*
  DIWEN_480.h - DIWEN 480 UART_LCD library for Arduino
add homebranche t 4-8 0-19
*/
#ifndef __DIWEN_480_H
#define __DIWEN_480_H

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
    void clr(uint16_t Color); //清屏
  private:
    void LCD_printf(uint8_t mode, uint16_t Color, uint16_t Bcolor, uint16_t x, uint16_t y, const char *format, ...); //显示字符串或者变量
    void erweima(uint16_t x, uint16_t y, uint8_t QR_Pixel, const char *format, ...);   //生成二维码
    void Linear_interpolation(uint8_t num, uint16_t Color, ...);        //绘制直线
    void Place_point(uint8_t num, uint16_t Color, uint16_t nx, uint16_t ny, ...); //绘制点
    //void Bode_Set(u16 Set);  //调节扩展串口波特率
    void disd_ico(uint16_t x, uint16_t y, uint8_t mode, uint8_t Icon_lib, uint8_t Icon_IDs, uint8_t Icon_0IDe, uint8_t Delay_time); //设置动态图标
    void dis_ico(uint16_t x, uint16_t y, uint8_t mode, uint8_t ids);                             //显示ico图标
    void pic(uint8_t id);                                                                        //写数据存储器
}

#endif