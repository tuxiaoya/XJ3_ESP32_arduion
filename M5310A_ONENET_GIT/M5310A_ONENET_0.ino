/*
  String length()

  Examples of how to use length() in a String.
  Open the Serial Monitor and start sending characters to see the results.
  created 1 Aug 2010
  by Tom Igoe

  This example code is 妗桄 掛華 GIT

  move  HWSG to GITHUB
  1-3

  http://www.arduino.cc/en/Tutorial/StringLengthTrim
*/
#include <HardwareSerial.h>
#include <Ticker.h>
#include  "HWSG2_uart.h"


// #include "nbiot_m5310.c"
 
#define dpx0 0
#define dpx1 400 //
#define dpx2 800 //rrrrrggggggbbbbb
#define dc_white  0xffff //         B1111111111111111
#define dc_red    0xf800 //    B1111100000000000
#define dc_gre    0x07e0 //    B0000011111100000
#define dc_blue   0x001f // B0000000000011111


uint64_t chipid;

struct NB_5310_RES
{
  bool status;
  String data;
  String temp;
};



Ticker ticker_HWSG;  // 
// Ticker tickerSetLow;

//String logo="MINGUANG CO.LTD";
//String inputString = "";         // a String to hold incoming data
//bool stringComplete = false;  // whether the string is complete

ESP  myesp32;

HardwareSerial M5310_Serial(0);
HardwareSerial DIWEN_Serial(1);
HardwareSerial HWSG_Serial(2);
bool ONENET_online = false;


//
//String txtMsg = "";                         // a string for incoming text
//unsigned int lastStringLength = txtMsg.length();     // previous length of the String

void setup() {

  chipid = myesp32.getEfuseMac();                                  //The chip ID is essentially its MAC address(length: 6 bytes).
  Serial.printf("ESP32 Chip ID = %04X", (uint16_t)(chipid >> 32)); //print High 2 bytes
  Serial.printf("%08X\n", (uint32_t)chipid);                       //print Low 4bytes.

  // Open serial communications and wait for port to open:
  M5310_Serial.begin(9600);  // �菔楝銝�霈斗�� t
  DIWEN_Serial.begin(115200, SERIAL_8N1, 17, 16);  //  rxPin, txPin
  HWSG_Serial.begin(1200, SERIAL_8N1, 18, 19);  //  rxPin, txPin
  while (!M5310_Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // send an intro:
  // M5310_Serial.println("\n\nString  length():");

  SetFace(0);
  LightOut(5);
  SetFace(1);
  LightIn(5); 

  DisMessage(0,"MINGUANG CO.LTD" ,dc_blue); // 
  DisMessage(1,"--HWSG NBIOT VCODE SYSTEM--" ,dc_blue); //    


  if(setup_M5310()) {
    DisMessage(0,"M5310 INIT Succese!",dc_white); 
    DisMessage(1,"New SIGN to ONENET ...........",dc_white);
    if(MIPLADDOBJ()){ // //�餃�撟喳
    DisMessage(0,"M5310 ADDOBJ Succese!",dc_white);         
      }    
    }
  else{
    DisMessage(0,"M5310 INIT  falsed!!!",dc_red); 
    DisMessage(1,"-------------",dc_white);    
    }

  ticker_HWSG.attach(1, Call_HWSG);   //瘥?1蝘��每all_HWSG  upload tem  
}

void loop() {

}



void DisMessage(char No_place,String msg,uint16_t dcc ){

    switch (No_place) {
    case 0:    //  
      DisStrings(0x42,dcc,0x0000,dpx0,460,msg);
      break;
    case 1:    // your hand is close to the sensor
      DisStrings(0x42,dcc,0x0000,dpx1,460,msg);
      break;
    case 2:    // your hand is close to the sensor
      DisStrings(0x42,dcc,0x0000,dpx2,460,msg);
      break;    
    case 3:    // your hand is close to the sensor
      DisStrings(0x41,dcc,0x0000,0,0,msg);
      break; 
  }  
}


//const String  AT_command_5310[4]={
//  "AT\r\n", //�斗璅∠��?�虫��萄��箸��?  
//  "AT+CSQ\r\n",// 靽∪韐券�璉�?
//  "AT+CEREG?\r\n",// ����嗆���雿��?
//  "AT+CGATT?\r\n",//璉�交芋蝏? PS ���嗆? 
//};
//AT+COPS=1,2,"46000" �//霈曄蔭�瘜典�蝘餃餈�? MNC
//OK
//AT+NEARFCN=0,3555 �//��憸銝? 3555嚗�憸隞交���撠�蝵�湛�雿憸霈曄蔭�??隡?潸��憭梯揖嚗遣霈?�虜�銝�閬?曄蔭�??
//OK
//AT+CSCON=1 �//��靽∪�內�?�其��?
//OK
//AT+CEREG=1 �//��瘜典�靽⊥�?�其��?
//OK
//const String  AT_command_5310[6]={
//  "AT\n", //�斗璅∠��?�虫��萄��箸��?  
//  "AT+CIMI\n",
//  "AT+CSQ\n",// 靽∪韐券�璉�?
//  "AT+CEREG?\n",// ����嗆���雿��?
//  "AT+CGATT?\n",//璉�交芋蝏? PS ���嗆?
//  "AT+CGATT?\n",//  
//};

const String  AT_command_5310[6]={
  "AT", //�斗璅∠��?�虫��萄��箸��?  
  "AT+CIMI",
  "AT+CSQ",// 靽∪韐券�璉�?
  "AT+CEREG?",// ����嗆���雿��?
  "AT+CGATT?",//璉�交芋蝏? PS ���嗆?
};


bool  setup_M5310()
{ NB_5310_RES  res;    
  DisMessage(0,"INIT M5310 AT",dc_white); //�曄內��蝚血     
  delay(2000);   
  res = wait_rx_bc(AT_command_5310[0],4000,"OK");  //  AT  --  OK 
     if(res.status) {
        DisMessage(1,"M5310 POWER OK",dc_gre);
        }
      else{                        
      DisMessage(1,"M5310 POWER ERROR! SETUP quit!",dc_red);   
      return (false);
      }      
  delay(2000);  
     
  delay(2000);
  // 銵函內�交靽∪撘箏漲���? 0~31嚗虜 CSQ�澆之鈭?12�?閫蛹蝵��賢?蝙�?
  res = wait_rx_bc(AT_command_5310[2],4000,"OK");  //   "AT+CSQ\n",  +CSQ:20,99 
   if(res.status) {
    char ri;
    ri=res.temp.substring(6,8).toInt();
    if((ri>12)&&(ri!=99))
      {DisMessage(1,"CSQ:"+res.temp.substring(6,8),dc_gre);}
     else{
      DisMessage(1,"No CSQ_sign,SETUP quit!"+res.data.substring(6,8),dc_red);  
      return (false);
      }
    }
    else{                        
    DisMessage(1,"CSQ REV ERROR! SETUP quit!",dc_red);   
    return (false);
      } 
    delay(2000);  
   
  res = wait_rx_bc(AT_command_5310[3],4000,"OK");  //  NB���嗆? +CEREG:0,1 (1-5)
   if(res.status) {
    char ri;
    ri=res.temp.substring(10).toInt();
    if((ri>=1)&&(ri<=5))
      {DisMessage(1,"NB+CEREG ="+res.temp.substring(10),dc_gre);}
     else{
      DisMessage(1,"NB+CEREG false,SETUP quit!",dc_red);  
      return (false);
      }
    }
    else{                        
    DisMessage(1,"NB+CEREG REV ERROR! SETUP quit!",dc_red);   
    return (false);
      } 
    delay(2000);
  res = wait_rx_bc(AT_command_5310[4],4000,"OK");  //  AT+CGATT?\n",//璉�交芋蝏? PS ���嗆?  :1 ok  0:false    
   if(res.status) {
    if(res.temp.substring(8).toInt()==1)
      {DisMessage(1,"CGATT="+res.temp.substring(8),dc_gre);
       DisMessage(0,"M5310 NB_IOT OK!!!",dc_gre);      
      }
     else{
      DisMessage(1,"CGATT false,SETUP quit!",dc_red);  
      return (false);
      }
    }
    else{                        
    DisMessage(1,"CGATT ERROR! SETUP quit!",dc_red);   
    return (false);
      } 
}


// 璅∠� 靘扯?曉?�撱綽��?���豢�靘�鋆末�釣�� 頧?�Ｗ極�? �釣�� 頧?�Ｘ�靘??

const String  AT_MIPLADDOBJ[4]={//�遣摰�  // 霈曄蔭璅∠�靘扯?曉?釣��
  "AT+MIPLCREATE=49,130031F10003F2002304001100000000000010123138332E3233302E34302E33393A35363833000131F300080000000000,0,49,0 ",  
  "AT+MIPLADDOBJ=0,3200,1,\"1\",0,1 ",   //撖寡情   靘�銝?嚗�霈ａ� Object 3200嚗nstance 0 銝箔�
  "AT+MIPLDISCOVERRSP=0,3200,1,4,\"5750\"",   //霈ａ� Resource韏� 靘�銝?隞?ａ� Object3200嚗nstance 0 �? 5750 Resource 銝箔�
  "AT+MIPLOPEN=0,3000,30" ,     //�餃�撟喳    
};
bool  MIPLADDOBJ()
{  NB_5310_RES  res;   
   res=wait_rx_bc(AT_MIPLADDOBJ[0],3000,"+MIPLCREATE:0"); //  //�遣摰� 霈曄蔭璅∠�靘扯?曉?釣�� 
    if(res.status) {
    DisMessage(1,"MIPLCREATE OK:"+res.temp,dc_gre);
    }
        else{                        
    DisMessage(1,"MIPLCREATE false嚗DDOBJ quit!",dc_red);   
    return (false);
      } 
    delay(2000);

   res=wait_rx_bc(AT_MIPLADDOBJ[1],3000,"OK"); //霈ａ� Object 撖寡情
    if(res.status) {
    DisMessage(1,"MMIPLADDOBJ OK:"+res.temp,dc_gre);
    }
        else{                        
    DisMessage(1,"ADDOBJ false嚗DDOBJ quit!",dc_red);   
    return (false);
      } 
    delay(2000);    

   res=wait_rx_bc(AT_MIPLADDOBJ[2],3000,"OK"); //霈ａ� Resource韏�   
    if(res.status) {
    DisMessage(1,"MIPLDISCOVERRSP OK:"+res.temp,dc_gre);
    }  
        else{                        
    DisMessage(1,"DISCOVERRSP false嚗DDOBJ quit!",dc_red);   
    return (false);
      } 
    delay(2000);
      
   res=wait_rx_bc(AT_MIPLADDOBJ[3],3000,"OK"); ////�餃�撟喳嚗?3000 隞�”�?甈⊥釣������瘣餅�? 蝘�30隞�”�餃�頞�園 蝘�
    if(res.status) {
    DisMessage(1,"MIPLOPEN OK:"+res.temp,dc_gre);    
    }  
        else{                        
    DisMessage(1,"MIPLOPEN false嚗DDOBJ quit!",dc_red);   
    return (false);
      } 
    delay(2000);  
  }

  /*
璅∠��餃�����撟喳隡��? OBSERVE 瘨�? DISCOVER 瘨嚗芋蝏
�啗�銝斗瘨銋�嚗��?�典?�嚗���冽�西??��?
[#Recv] +MIPLOBSERVE:0,39121,1,3200,0,-1
[#Recv] +MIPLDISCOVER:0,39122,3200
[#Recv] +MIPLREAD:0,39123,3,0,-1,0
�? 摰�憭�銝?嚗���?+MIPLEVENT:0,6 瘨 �亦＆霈斗芋蝏�衣敶?
OneNET 撟喳�� 嚗? 撟嗡�嚗? �閬�敺芋蝏? �亙��? �? OBSERVE 瘨�? �? DISCOVER
瘨 �? 嚗�?�扯?�? �嗅� ��
*/

const String  AT_MIPLNOTIFY[4]={                    // OneNET �唳�嗅�瘚�
"AT+MIPLNOTIFY=0,0,3200,0,5750,1,4,\"177788\",0,0,12", //嚗蒂ackid嚗ckid瘥?∩��亥?��銝�唳   
"AT+MIPLREAD=0,60204,3200,0,5505,\"E309C82FE6\",1",     //Read ���?�蝔?
"AT+MIPLWRITE=0,62069,1",                             //Write ���?�蝔?
"AT+MIPLEXECUTE=0,46081,1",                           //Execute ���?�蝔?    
};

const String  AT_MIPLCLOSE[3]={                   // OneNET �餃�瘜券�瘚�
"AT+MIPLCLOSE=0",           //�餃�瘜券�瘚�
"AT+MIPLDELOBJ=0,3200,0",   //璅∠�靘扯?ａ�韏��”�
"AT+MIPLDEL=0",             //璅∠�靘折縑摰��  
};


// �T �誘嚗�敺��憭�餈�蝏�雿? NB_5310_RES
//struct NB_5310_RES
//{
//  bool status;  �?��?
//  String data;  餈���蝚虫葡
//  String temp;  銝湔 
//};
#define trytime 3
NB_5310_RES wait_rx_bc(String str_ATcmd,long tout,String str_wait)
{
  unsigned long pv_ok = millis();
  unsigned long current_ok = millis();
  String input;
  unsigned char flag_out=1;
  NB_5310_RES res_; 
  bool res= false;
  res_.temp="";
  res_.data = "";

  while(M5310_Serial.read() >= 0);
  while(flag_out<trytime)
  {
    M5310_Serial.println(str_ATcmd);   //�T�賭誘
    DisMessage(0,"SEND Com="+str_ATcmd,dc_white); //�曄內��command 
    if( M5310_Serial.available())   //銝脣��憭?
    {
      input = M5310_Serial.readStringUntil('\n');      
      res_.temp+=input;      
      if(input.indexOf(str_wait)!=-1)//甇�＆�嗅
      {
        res= true;
        res_.status = res;
        res_.data = input;   
        DisMessage(3,str_ATcmd+"---"+res_.temp ,dc_white); //甇�＆�嗅       
   
        return(res_);               
      }
        else if(input.indexOf(F("ERROR"))!=-1)
      {
        res=false;
        flag_out++;
        res_.temp="";
        res_.data = "";              
      }
    }
    current_ok = millis();
    if (current_ok - pv_ok>= tout)
    {
      // flag_out=5;
      res=false;
      res_.status = res;
      res_.data ="AT OUT time" ;
      while(M5310_Serial.read() >= 0);
      DisMessage(3,"AT OUT time!--" ,dc_red); // 頞
      return(res_);    
    }
  }

}


void Call_HWSG()
{ static bool  hh=true; 
  HWSG_Serial.write(HWSG_CMD_Upload_tem);  // send 0xc0 2 times
  HWSG_Serial.write(HWSG_CMD_Upload_tem);  
  hh=!hh;
  if(hh){
    DisMessage(2,"-" ,dc_red); // 
    }
    else{DisMessage(2,"|" ,dc_red); }//  
}



//AA 11   41    FF FF   00 00 00 20     -------------   CC 33 C3 3C
  
//Mode嚗蝷箸芋撘?
//.7 摮?血?賢漲靚霈曄蔭 1=靚 0=銝��氬?
//.6 ��脫蝷箄?曄蔭 1=�曄內 0=銝蝷箝?
//.5-.4 �?0�?
//.3-.0嚗��瑕之撠�0x00-0x09嚗?孵�摮�憭批�鈭�嚗?
//0x00=6*12 0x01=8*16 0x02=10*20 0x03=12*24 0x04=14*28
//0x05=16*32 0x06=20*40 0x07=24*48 0x08=28*56 0x09=32*64
//Color嚗�蝚行蝷粹?�?
//Bcolor嚗�蝚西��?�曄內憸�?
//嚗嚗嚗�摮?虫葡�曄內�椰銝?��?//string.getBytes(buf, len)
//Strings嚗?蝷箇�摮?虫葡嚗�ASCII 摮?行��?GBK 蝻��澆�瘙��曄內�?
void DisStrings(uint8_t smode,uint16_t cc,uint16_t dc,uint16_t x,uint16_t y, String dstr) //    
{ uint8_t dstart[] = {0xAA, 0x11, 0x41, 
  0xff,0xff, 0x00, 0x00, 
  0x0,0x20, 0x01,0x50 }; 
  uint8_t dend[] ={0xCC, 0x33, 0xC3, 0x3C };
  uint8_t ds[50];
  for(int i=0; i<50;i++){
    ds[i]=' ';
    }
  dstr.getBytes(ds, dstr.length()+1);

  dstart[2]=smode;dstart[3]=cc>>8;dstart[4]=cc&0x00FF;dstart[5]=dc>>8;dstart[6]= dc&0x00FF;
  dstart[7] = uint8_t(x >> 8);
  dstart[8] = uint8_t(x & 0x00FF);
  dstart[9] = uint8_t(y >> 8);
  dstart[10] = uint8_t(y & 0x00FF);

  DIWEN_Serial.write(dstart,11);  
  DIWEN_Serial.write(ds,39);  //DIWEN_Serial.write(ds,dstr.length()+1);  
  DIWEN_Serial.write(dend,4);  
 }


void   SetFace(uint8_t face)
{ //AA 22 00 00 CC 33 C3 3C
  uint8_t Face0[] = {0xAA, 0x22, 0x00, 0x00, 0xCC, 0x33, 0xC3, 0x3C };
  Face0[3] = face;
  DIWEN_Serial.write(Face0,8);  
 }


//DIM_Set嚗��漁摨血潘�0x00-0xFF�?
//0x00 ���喲嚗?0xFF ���鈭?嚗銝?0x01-0x1F 霈曄蔭�潸��?
//�?�賭��?�?
//銝暺?文潭0xFF�?
//銝曆�嚗A 30 80 CC 33 C3 3C 鈭?摨西��50%�?
void   SetLightness(uint8_t Light)
{ 
  uint8_t Lightness[] = {0xAA, 0x30, 0xff, 0xCC, 0x33, 0xC3, 0x3C };
  Lightness[2] = Light;
  DIWEN_Serial.write(Lightness,7);  
 }

 void   LightIn(uint8_t lspeed)
{  
  for (  uint8_t  i=0 ;i<255;i++)
  {
  delay(lspeed);
  SetLightness(i);  
  }
 }

  void   LightOut(uint8_t lspeed)
{  
  for (  uint8_t  i=0 ;i<255;i++)
  {
  delay(lspeed);
  SetLightness(255-i);  
  }
 }
