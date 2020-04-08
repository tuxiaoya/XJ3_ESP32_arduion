
extern "C"
{
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
}

#include <HardwareSerial.h>
#include "DIWEN_480.h"

DIWEN480::DIWEN480()
{
    HardwareSerial DIWEN_Serial(1);
}

void DIWEN480::Begin()
{
    // send an intro:
    // M5310_Serial.println("\n\nString  length():");
    SetFace(0);
    LightOut(5);
    SetFace(1);
    LightIn(5);
    DisMessage(0, "MINGUANG CO.LTD", dc_blue);             //
    DisMessage(1, "--HWSG NBIOT VCODE SYSTEM--", dc_blue); //
}

//AA 11   41    FF FF   00 00 00 20     -------------   CC 33 C3 3C
//Mode：显示模式。
//.7 字符宽度调整设置 1=调整 0=不调整。
//.6 背景色显示设置 1=显示 0=不显示。
//.5-.4 写0。
//.3-.0：字号大小，0x00-0x09，对应字体大小于下：
//0x00=6*12 0x01=8*16 0x02=10*20 0x03=12*24 0x04=14*28
//0x05=16*32 0x06=20*40 0x07=24*48 0x08=28*56 0x09=32*64
//Color：字符显示颜色。
//Bcolor：字符背景显示颜色。
//（x，y）：字符串显示的左上角坐标。//string.getBytes(buf, len)
//Strings：要显示的字符串，非ASCII 字符按照GBK 编码格式汉字显示。
void DIWEN480::DisStrings(uint8_t smode, uint16_t cc, uint16_t dc, uint16_t x, uint16_t y, String dstr) //
{
    uint8_t dstart[] = {0xAA, 0x11, 0x41,
                        0xff, 0xff, 0x00, 0x00,
                        0x0, 0x20, 0x01, 0x50};
    uint8_t dend[] = {0xCC, 0x33, 0xC3, 0x3C};
    uint8_t ds[50];
    for (int i = 0; i < 50; i++)
    {
        ds[i] = ' ';
    }
    dstr.getBytes(ds, dstr.length() + 1);

    dstart[2] = smode;
    dstart[3] = cc >> 8;
    dstart[4] = cc & 0x00FF;
    dstart[5] = dc >> 8;
    dstart[6] = dc & 0x00FF;
    dstart[7] = uint8_t(x >> 8);
    dstart[8] = uint8_t(x & 0x00FF);
    dstart[9] = uint8_t(y >> 8);
    dstart[10] = uint8_t(y & 0x00FF);

    DIWEN_Serial.write(dstart, 11);
    DIWEN_Serial.write(ds, 39); //DIWEN_Serial.write(ds,dstr.length()+1);
    DIWEN_Serial.write(dend, 4);
}

void DIWEN480::SetFace(uint8_t face)
{ //AA 22 00 00 CC 33 C3 3C
    uint8_t Face0[] = {0xAA, 0x22, 0x00, 0x00, 0xCC, 0x33, 0xC3, 0x3C};
    Face0[3] = face;
    DIWEN_Serial.write(Face0, 8);
}

//DIM_Set：背光亮度值，0x00-0xFF。
//0x00 背光关闭，0xFF 背光最亮，其中0x01-0x1F 设置值背光
//可能会闪烁。
//上电默认值是0xFF。
//举例：AA 30 80 CC 33 C3 3C 亮度调节到50%。
void DIWEN480::SetLightness(uint8_t Light)
{
    uint8_t Lightness[] = {0xAA, 0x30, 0xff, 0xCC, 0x33, 0xC3, 0x3C};
    Lightness[2] = Light;
    DIWEN_Serial.write(Lightness, 7);
}

void DIWEN480::LightIn(uint8_t lspeed)
{
    for (uint8_t i = 0; i < 255; i++)
    {
        delay(lspeed);
        SetLightness(i);
    }
}

void DIWEN480::LightOut(uint8_t lspeed)
{
    for (uint8_t i = 0; i < 255; i++)
    {
        delay(lspeed);
        SetLightness(255 - i);
    }
}
