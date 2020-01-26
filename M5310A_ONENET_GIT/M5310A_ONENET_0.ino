/*
  String length()

  Examples of how to use length() in a String.
  Open the Serial Monitor and start sending characters to see the results.
  created 1 Aug 2010
  by Tom Igoe

  This example code is 婵℃顢� 閹烘稖褰� GIT

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
  M5310_Serial.begin(9600);  // 閿熷€熷絽濡ゆ繈濮栭姗堟嫹闂囧牊鏋熼鏃撴嫹椁佹牭鎷� t
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
    if(MIPLADDOBJ()){ // //閿熶粙顦ㄩ敓鑺ユ寭閸犳剚寰�
    DisMessage(0,"M5310 ADDOBJ Succese!",dc_white);         
      }    
    }
  else{
    DisMessage(0,"M5310 INIT  falsed!!!",dc_red); 
    DisMessage(1,"-------------",dc_white);    
    }

  ticker_HWSG.attach(1, Call_HWSG);   //閻︼拷?1閾︽﹩妲嬮敓鏂ゆ嫹濮ｅ话ll_HWSG  upload tem  
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
//  "AT\r\n", //閿熻姤鏋熺潰鍕嫕閳媴鎷烽敓锟�?閿熷€熸珪閿熸枻鎷烽拃鍕舵嫹閿熺晫顔忛敓鏂ゆ嫹?  
//  "AT+CSQ\r\n",// 闂堣В鍩呴鍐厙閸掗潻鎷烽悹澹溾偓閿燂拷?
//  "AT+CEREG?\r\n",// 閿熺瓔绔婚敓鏂ゆ嫹鑱欓敓钘夋⒖鑱欑奖鍜冩嫹閿熸枻瀚敓浠嬫禌椁佹熬鎷烽敓锟�?
//  "AT+CGATT?\r\n",//閻犲鈧敓鎴掓唉閼哄娼�? PS 閿熸枻鍓堕敓鏂ゆ嫹閸″枴鈧�? 
//};
//AT+COPS=1,2,"46000" 閿燂拷//闂囧牊娉熼拕顓ㄦ嫹椋兼繐鎷戦惁婊冨悁閿熷€熸剑妞佸喛鎷戞鍫惣椤犲函鎷�? MNC
//OK
//AT+NEARFCN=0,3555 閿燂拷//閿熺瓔钃熼敓鑺ュ枑椤ｎ収绮抽柕锟�? 3555閸ゆ妞岄敓鑺ュ枑椤ｎ偒寰庨梾鐐版唉閿熸枻鎷烽５鍡嫹閹剧媭鎯掗敓鍊熸緥椤ｎ剦锛婇敓鑺ョ閿熶粙娴涢０宥庮洢閹插壊鏆涚焦顒勬箑閺囧嫯鏁敓绛嬨€�??闂呪槅顏�?濞奸潻濮奸敓绛嬭壘閿熻姤鍟冲顖涘絿閸ゆ妞囬柆锝夋箑?閿熺瓔顏搹婊愭嫹椋垽鍨抽柕婵撴殔閿熶粙鏌嗙酱锟�?閺囧嫯鏁敓绛嬩紟??
//OK
//AT+CSCON=1 閿燂拷//閿熺瓔娈介敓浠嬫緰閳亶鎮嗛敓绛嬫惢閸徰嶆嫹?閿熻棄鍙鹃敓鏂ゆ嫹?
//OK
//AT+CEREG=1 閿燂拷//閿熺瓔娈介敓鐣屾В閸忛潻鎷烽棃瑙ｅГ缃村拑鎷�?閿熻棄鍙鹃敓鏂ゆ嫹?
//OK
//const String  AT_command_5310[6]={
//  "AT\n", //閿熻姤鏋熺潰鍕嫕閳媴鎷烽敓锟�?閿熷€熸珪閿熸枻鎷烽拃鍕舵嫹閿熺晫顔忛敓鏂ゆ嫹?  
//  "AT+CIMI\n",
//  "AT+CSQ\n",// 闂堣В鍩呴鍐厙閸掗潻鎷烽悹澹溾偓閿燂拷?
//  "AT+CEREG?\n",// 閿熺瓔绔婚敓鏂ゆ嫹鑱欓敓钘夋⒖鑱欑奖鍜冩嫹閿熸枻瀚敓浠嬫禌椁佹熬鎷烽敓锟�?
//  "AT+CGATT?\n",//閻犲鈧敓鎴掓唉閼哄娼�? PS 閿熸枻鍓堕敓鏂ゆ嫹閸″枴鈧�?
//  "AT+CGATT?\n",//  
//};

const String  AT_command_5310[6]={
  "AT", //閿熻姤鏋熺潰鍕嫕閳媴鎷烽敓锟�?閿熷€熸珪閿熸枻鎷烽拃鍕舵嫹閿熺晫顔忛敓鏂ゆ嫹?  
  "AT+CIMI",
  "AT+CSQ",// 闂堣В鍩呴鍐厙閸掗潻鎷烽悹澹溾偓閿燂拷?
  "AT+CEREG?",// 閿熺瓔绔婚敓鏂ゆ嫹鑱欓敓钘夋⒖鑱欑奖鍜冩嫹閿熸枻瀚敓浠嬫禌椁佹熬鎷烽敓锟�?
  "AT+CGATT?",//閻犲鈧敓鎴掓唉閼哄娼�? PS 閿熸枻鍓堕敓鏂ゆ嫹閸″枴鈧�?
};


bool  setup_M5310()
{ NB_5310_RES  res;    
  DisMessage(0,"INIT M5310 AT",dc_white); //閿熻姤娉熼崗褝鎷烽。顕封偓缃存枻鎷烽摝姘愁攨椤烇拷     
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
  // 闁甸潧鍤遍崗褝鎷锋禍銈庢箯闂堣В鍩呴鍐╂寠缁犲繑鐤鹃敓绛嬨€戦敓鏂ゆ嫹? 0~31閸ゆ妞岃仚鐬版媽娅� CSQ閿熻姤绶犳稊瀣焾?12閿燂拷?闂侇偓澧滈摂纭呮緥椤ｎ叏鎷烽敓鍊熷皾?缃♀偓閾︽瑱鎷�?
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
   
  res = wait_rx_bc(AT_command_5310[3],4000,"OK");  //  NB閿熸枻鍓堕敓鏂ゆ嫹閸″枴鈧�? +CEREG:0,1 (1-5)
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
  res = wait_rx_bc(AT_command_5310[4],4000,"OK");  //  AT+CGATT?\n",//閻犲鈧敓鎴掓唉閼哄娼�? PS 閿熸枻鍓堕敓鏂ゆ嫹閸″枴鈧�?  :1 ok  0:false    
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


// 閻犲應鍨归敓锟� 闂堟ɑ澹�?閺囷拷?椋测晪鎷烽幘杈╃潱閿熸枻鎷�?閿熸枻娈敓鏂ゆ嫹鐠烆澁鎷烽棃姗嗙暛閿熶粙濯堕－顖涙汞閿熸枻鍓甸柌锝忔嫹椋剧》鎷� 闂嬶拷?閿熸枻鎸ゅΔ纰夋嫹? 閿熸枻妞堥柌锝忔嫹椋剧》鎷� 闂嬶拷?閿熸枻鍑犻敓浠嬫剑鐢猴拷??

const String  AT_MIPLADDOBJ[4]={//閿熺瓔鐣柆锝嗘嚀鐣癸綇鎷�  // 闂囧牊娉熼拕顓犳嫕閳媴鎷烽棃妯诲?閺囷拷?椋差亪鍤曢敓鏂ゆ閿燂拷
  "AT+MIPLCREATE=49,130031F10003F2002304001100000000000010123138332E3233302E34302E33393A35363833000131F300080000000000,0,49,0 ",  
  "AT+MIPLADDOBJ=0,3200,1,\"1\",0,1 ",   //閹炬牕顕栭幆锟�   闂堟﹫鏆夐敓浠嬪?閸ゆ妞嗛敓浠嬫箑閿濅緤鎷� Object 3200閸ゆ鏋瞡stance 0 闁垫繄鐣堕敓锟�
  "AT+MIPLDISCOVERRSP=0,3200,1,4,\"5750\"",   //闂囧牞绲归敓锟� Resource闂婂骏鍓甸敓锟� 闂堟﹫鏆夐敓浠嬪?闂呯儑鍟�?閿濅緤鎷� Object3200閸ゆ鏋瞡stance 0 閿燂拷? 5750 Resource 闁垫繄鐣堕敓锟�
  "AT+MIPLOPEN=0,3000,30" ,     //閿熶粙顦ㄩ敓鑺ユ寭閸犳剚寰�    
};
bool  MIPLADDOBJ()
{  NB_5310_RES  res;   
   res=wait_rx_bc(AT_MIPLADDOBJ[0],3000,"+MIPLCREATE:0"); //  //閿熺瓔鐣柆锝嗘嚀鐣癸綇鎷� 闂囧牊娉熼拕顓犳嫕閳媴鎷烽棃妯诲?閺囷拷?椋差亪鍤曢敓鏂ゆ閿燂拷 
    if(res.status) {
    DisMessage(1,"MIPLCREATE OK:"+res.temp,dc_gre);
    }
        else{                        
    DisMessage(1,"MIPLCREATE false閸ゆ鏆〥DOBJ quit!",dc_red);   
    return (false);
      } 
    delay(2000);

   res=wait_rx_bc(AT_MIPLADDOBJ[1],3000,"OK"); //闂囧牞绲归敓锟� Object 閹炬牕顕栭幆锟�
    if(res.status) {
    DisMessage(1,"MMIPLADDOBJ OK:"+res.temp,dc_gre);
    }
        else{                        
    DisMessage(1,"ADDOBJ false閸ゆ鏆〥DOBJ quit!",dc_red);   
    return (false);
      } 
    delay(2000);    

   res=wait_rx_bc(AT_MIPLADDOBJ[2],3000,"OK"); //闂囧牞绲归敓锟� Resource闂婂骏鍓甸敓锟�   
    if(res.status) {
    DisMessage(1,"MIPLDISCOVERRSP OK:"+res.temp,dc_gre);
    }  
        else{                        
    DisMessage(1,"DISCOVERRSP false閸ゆ鏆〥DOBJ quit!",dc_red);   
    return (false);
      } 
    delay(2000);
      
   res=wait_rx_bc(AT_MIPLADDOBJ[3],3000,"OK"); ////閿熶粙顦ㄩ敓鑺ユ寭閸犳剚寰忛崵锟�?3000 闂呯儑鎷烽垾婵撴嫹?閻㈠牃濮ら柌锝忔嫹椋句紮鎷烽敓绛嬬閿熸枻鎷烽＋鎺炴嫹閻︼綁顦澘顏庢嫹? 閾︽﹩妲栭敓锟�30闂呯儑鎷烽垾婵撴嫹妞佸喛鎷烽棆鐑囧暫鐫款亷鎷烽崷鎺濓紙 閾︽﹩妲栭敓锟�
    if(res.status) {
    DisMessage(1,"MIPLOPEN OK:"+res.temp,dc_gre);    
    }  
        else{                        
    DisMessage(1,"MIPLOPEN false閸ゆ鏆〥DOBJ quit!",dc_red);   
    return (false);
      } 
    delay(2000);  
  }

  /*
閻犲應鍨归敓鏂ゆ嫹妞佸喛鎷烽敓绛嬫惛閿熸枻鎷烽鐑囨嫹閹剧喎鏌嶉濂告鐬邦垽鎷烽敓锟�? OBSERVE 閻︻煉宸肩酱鍜冩嫹? DISCOVER 閻︻煉宸肩酱鍛鐢哄氦濡洪摝蹇ュ壍椤拷
閿熻棄鏅介敓浠嬪閺傛鐭侀惁顭掑芳缃村懘濡洪＜婵撴嫹閸ゆ妞嗛敓鏂ゆ嫹?閿熻棄鍚€?椋棑鎷烽崵妤嬫閿熸枻鎷疯仚閿熻棄鍠楅４姗堟嫹鐟楋拷?椋撅拷?椋棑鎷烽敓锟�?
[#Recv] +MIPLOBSERVE:0,39121,1,3200,0,-1
[#Recv] +MIPLDISCOVER:0,39122,3200
[#Recv] +MIPLREAD:0,39123,3,0,-1,0
閿燂拷? 閹介偊鑳嗛敓鑺ュ暢椋棑鎷烽柕锟�?閸ゆ妞嗛敓鏂ゆ嫹瑭堢儑宕婇敓锟�?+MIPLEVENT:0,6 閻︻煉宸肩酱锟� 閿熸垝瀹抽敍鍡涙箑閺傛濡洪摝蹇ュ壍鐬€鈧敓鍊熴€傜瀾鈺傛殽?
OneNET 閹剧喎鏌嶉纭锋嫹椤″函鎷� 閸わ拷? 閹剧喎妫ラ敓钘夋? 閿熼摪鈧梺顒夊唨閿熻姤鏆－鎷屽閾︼拷? 閿熸垝绨烽敓鏂ゆ嫹? 閿燂拷? OBSERVE 閻︻煉宸肩酱鍜冩嫹? 閿燂拷? DISCOVER
閻︻煉宸肩酱锟� 閿燂拷? 閸ゆ妞堢潰鎰舵嫹?閿熻姤澹�?椋炬椿鐏欓敓锟�? 閿熻棄姊鹃敓锟� 閿熸枻娈╅敓锟�
*/

const String  AT_MIPLNOTIFY[4]={                    // OneNET 閿熻棄鏁甸鐙呮嫹閸″拑鎷烽惁姘煎唨閿燂拷
"AT+MIPLNOTIFY=0,0,3200,0,5750,1,4,\"177788\",0,0,12", //閸ゆ妞囬拏淇沜kid閸ゆ鏋燾kid閻︺儺鎯�?閳晪鎷烽敓鎴掍亥?缃存拝濮伴敓绛嬮叄閿熶粙濮栭：渚婃媰閿熻棄鏁甸锟�   
"AT+MIPLREAD=0,60204,3200,0,5505,\"E309C82FE6\",1",     //Read 閿熸枻娈╅敓鏂ゆ嫹鐣癸拷?椁佹﹫鎷烽摝锟�?
"AT+MIPLWRITE=0,62069,1",                             //Write 閿熸枻娈╅敓鏂ゆ嫹鐣癸拷?椁佹﹫鎷烽摝锟�?
"AT+MIPLEXECUTE=0,46081,1",                           //Execute 閿熸枻娈╅敓鏂ゆ嫹鐣癸拷?椁佹﹫鎷烽摝锟�?    
};

const String  AT_MIPLCLOSE[3]={                   // OneNET 閿熶粙顦ㄩ敓鐣屾В閸掗潻鎷烽惁姘煎唨閿燂拷
"AT+MIPLCLOSE=0",           //閿熶粙顦ㄩ敓鐣屾В閸掗潻鎷烽惁姘煎唨閿燂拷
"AT+MIPLDELOBJ=0,3200,0",   //閻犲應鍨归敓浠嬫剑閹碉拷?閿濅緤鎷烽棅蹇ュ壍閿熸枻鎷风瀺婧锯偓婵撴嫹椋轰緡婀�
"AT+MIPLDEL=0",             //閻犲應鍨归敓浠嬫剑閹舵◣鈧灠顖滅瑦閹介偊鎷呴敓鏂ゆ嫹瑭堛埄鈯�  
};


// 閿熺瓔鏆炶仚瑭堢瓗 閿熸枻瀚х懢妯烘椋剧》鎷烽弫鐚村暫椤涙棑鎷风瀺娆欐嫹閹差叏娈归敓浠嬵槶椤偊鎷烽摝蹇ｆ閿熶粙娴�? NB_5310_RES
//struct NB_5310_RES
//{
//  bool status;  閿熺瓔涓�?椁佹﹫鎷烽敓锟�?
//  String data;  妞佸牜娼呴敓鏂ゆ嫹椋帪鎷烽摝姘虫珪閽侊拷
//  String temp;  闁垫繃绠剁澘锟� 
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
    M5310_Serial.println(str_ATcmd);   //閿熺瓔鏆炶仚瑭堢瓗閿熷€熸妱鐟撅拷
    DisMessage(0,"SEND Com="+str_ATcmd,dc_white); //閿熻姤娉熼崗褝鎷烽。顕封偓缃存枻鎷穋ommand 
    if( M5310_Serial.available())   //闁垫繆鍔婇顖ゆ嫹椋凤綇鎷烽幉锟�?
    {
      input = M5310_Serial.readStringUntil('\n');      
      res_.temp+=input;      
      if(input.indexOf(str_wait)!=-1)//閻㈠浄鎷烽敍鍡嫹閸″拑宕�
      {
        res= true;
        res_.status = res;
        res_.data = input;   
        DisMessage(3,str_ATcmd+"---"+res_.temp ,dc_white); //閻㈠浄鎷烽敍鍡嫹閸″拑宕�       
   
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
      DisMessage(3,"AT OUT time!--" ,dc_red); // 闂嬬儑鍟虹澘锟�
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
  
//Mode閸ゆ顏瀫蹇氭緪缁犳瓕濡洪幘姗嗘儙鑱�?
//.7 閹筋噮顧€?鐞涒偓?鐠┿垺鐤鹃棃姘句紦椤噣婀€閺囧嫯鏁� 1=闂堟熬浼撻‖锟� 0=闁垫繐娈敓鏂ゆ嫹濮橆儩鈧�?
//.6 閿熸枻妞堢瀾婵撴嫹閼搭偒顩撮摝椋庣暋?閺囧嫯鏁� 1=閿熻姤娉熼崗锟� 0=闁垫繐娈瀫蹇氭緪缁犳緵鈧�?
//.5-.4 閿燂拷?0閿燂拷?
//.3-.0閸ゆ顏敓鏂ゆ嫹閻熸洑绠ｉ幘鐙€鎯㈤敓锟�0x00-0x09閸ゆ妞�?鐎涚鎷烽幗顕咁樈閿熻姤鍟抽幍鐧告嫹闁愁収甯楅敓钘夋?
//0x00=6*12 0x01=8*16 0x02=10*20 0x03=12*24 0x04=14*28
//0x05=16*32 0x06=20*40 0x07=24*48 0x08=28*56 0x09=32*64
//Color閸ゆ顏敓鍊熸渐鐞涘矈顩撮摝椋庤ˉ?鐣庮叏褰撮敓锟�?
//Bcolor閸ゆ顏敓鍊熸渐鐟楀尅鎷烽敓锟�?閿熻姤娉熼崗褎鍠愮晭顓ㄥ酱閿燂拷?
//閸ゆ灏伴崵妤嬫煀閸ゆ鎯岄敓鑺ユ噺鐬嶏拷?閾忣偉鎲介敓鑺ユ碂閸徰嶆嫹椋帗銇愰柕婵撴惄?椤﹀牞鎷烽敓鏂ゅ鑱�?//string.getBytes(buf, len)
//Strings閸ゆ顏�?缃寸⒈顩撮摝椋庣暏閿熻姤鎳忕瀺锟�?閾忣偉鎲介崵妤嬫閿熺锤SCII 閹筋噮顧€?鐞涘矉鎷烽敓锟�?GBK 閾︽椿銆愰敓鏂ゆ嫹濠㈠棴鎷烽惁娆欐偩閿熸枻鎷烽弴鍕弾閿燂拷?
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


//DIM_Set閸ゆ顏伴敓鏂ゆ嫹椋枫垺绱掗幗銊攨鑱欏姗堟嫹0x00-0xFF閿燂拷?
//0x00 閿熸枻妞囬敓鏂ゆ嫹閸犲樊锛侀崵锟�?0xFF 閿熸枻妞囬敓鏂ゆ嫹鑱欓柍锟�?閸ゆ妞囬－鈧柕锟�?0x01-0x1F 闂囧牊娉熼拕顓ㄦ嫹濞奸潻鎷烽敓锟�?
//閿燂拷?閿熷€熸妱閿熸枻鎷�?閿熺瓔鑴婅仚?
//闁垫繐鎼ㄩ—鎺撴鐬€锟�?閺傚毝鈧顓ь洢0xFF閿燂拷?
//闁垫繃娉﹂敓钘夋鐬熺摰 30 80 CC 33 C3 3C 闁筹拷?閹姐劏銈块敓鏂ゆ嫹缇佹﹫宕�50%閿燂拷?
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
