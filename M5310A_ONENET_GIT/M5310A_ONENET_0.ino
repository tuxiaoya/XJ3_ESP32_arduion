/*
  String length()

  Examples of how to use length() in a String.
  Open the Serial Monitor and start sending characters to see the results.
  created 1 Aug 2010
  by Tom Igoe

  This example code is 婵犵鍓濋〃鍡涖€侀敓锟� 闂佸湱鍎ょ粙鏍亹閿燂拷 GIT

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
  M5310_Serial.begin(9600);  // 闂佽法鍠庨埀顒傚枎缁茶姤淇婇妶鍡欑疄濠殿喗鐗犻、鈥愁潨閸喎顏堕梻鍌氭搐閻楀﹪寮搁悢纰辨晹闁哄啯鎸荤€氳顦︽担鍦闁瑰嚖鎷� t
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
    if(MIPLADDOBJ()){ // //闂佽法鍠嶇划娆撍囬妸鈺傛櫢闁兼亽鍎茬€殿參鏌涢悩鍐插鐎靛府鎷�
    DisMessage(0,"M5310 ADDOBJ Succese!",dc_white);         
      }    
    }
  else{
    DisMessage(0,"M5310 INIT  falsed!!!",dc_red); 
    DisMessage(1,"-------------",dc_white);    
    }

  ticker_HWSG.attach(1, Call_HWSG);   //闂佷紮璁ｉ幏锟�?1闂佹拝闄勯敃鈺佄ｇ€ｎ喗鏅搁柡鍌樺€栫€氳鎱ㄩ敐鍛扮樈ll_HWSG  upload tem  
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
//  "AT\r\n", //闂佽法鍠曟慨銈夊几閻旂儤妗ㄩ柛鏇ㄥ幗鐎氭洟鏌嶉鐘差€撻柟椋庡厴閺佹捇鏁撻敓锟�?闂佽法鍠庨埀顒傚枑閻濐亪鏌ㄩ悢鍛婄伄闁归鍏橀幏鍐礋閼搁潧顏堕梺璺ㄥ枔閺咁偊顢欒箛娑欐櫢闁哄倶鍊栫€氾拷?  
//  "AT+CSQ\r\n",// 闂傚倸鐗愯闁糕晛鎳橀、锝夊礃椤旀儳甯ｉ梺鍛婂浮濞煎骞忛悜鑺ュ仯濠㈣澹曢崑鎾绘煥閻曞倹瀚�?
//  "AT+CEREG?\r\n",// 闂佽法鍠撻悺鏃傜博婵犳碍鏅搁柡鍌樺€栫€氬綊鎳曞▎鎾存櫢闁芥ê顦伴埛鏍嚂濞嗘垵顨涢柛婊冨暞鐎氬綊鏌ㄩ悢鍛婄伄閻庢凹鍙冮弫鎾寸鐎ｎ偆顩卞浣风劍閻旑剟骞忛悜鑺ユ櫢闁跨噦鎷�?
//  "AT+CGATT?\r\n",//闂佽崵濮撮ˉ锟犲焵椤掑嫭鏅搁柟瀛樺笚閸炲鏌ら崫鍕靛創婵炵》鎷�? PS 闂佽法鍠愰弸濠氬礈閸洘鏅搁柡鍌樺€栫€氬綊鏌涢垾铏仱闁逞嶆嫹? 
//};
//AT+COPS=1,2,"46000" 闂佽法鍣﹂幏锟�//闂傚倸娲ら悧濠傗枖閻斿吋瀚婃い鎾卞妽鐎氳顦伴崗鑲╃闁归攱鍨块幆浣糕堪閸愩劍鍊庨梺璺ㄥ枎閳ь剛鍠愰崜鎴濐渻娴ｇǹ鏋﹂柟閿嬪灦椤︼箓宕奸锝嗗劘濡炪倗濮撮崙浠嬪箯閿燂拷? MNC
//OK
//AT+NEARFCN=0,3555 闂佽法鍣﹂幏锟�//闂佽法鍠撻悺鏃堟嫅閻斿吋鏅搁柤鎭掑劚閺嬫垶銇勯敐搴″季缂侇喗濞婇弻鏇㈡晸閿燂拷? 3555闂佹悶鍊栭〃鎰渻瀹€鍕櫢闁兼亽鍎遍弸鎴炪亜閿濆骸浜界€垫澘閰ｅ楣冩倷閻楀牆鏀梺璺ㄥ枑閺嬪骞忛悜鏂ょ穿闁糕槅鍘界€氬綊鏌熼崜褍顎岄柟顖涘浮閺佹捇宕愰悢鍝ヤ紣濡炪倧缍嗛崜锕傛晬婵犲洦鏅搁柤鎭掑劤椤旀洟鏌ㄩ悢鍓佺煓婵炲瓨鐩敍鎰吋鎼搭喗鍌ㄩ梺瑙勫絻婢瑰﹪寮冲☉姘卞妽妞ゆ帒瀚粻鎴︽煛閸パ冾伂闁轰緡浜弫鎾剁驳鐎ｃ劉鍋撻敓锟�??闂傚倸鎳夊Σ鍛搭敂閿燂拷?濠电偛銈稿缁樻叏婵傚憡鏅哥紒娑橆儓婢规﹢鏌ㄩ悢璇残撻柛鐔峰暱椤繘顢涘☉妯兼偧闂佹悶鍊栭〃鎰渻閸ヮ剚鐒奸柨婵嗩槹缁狅拷?闂佽法鍠撻悺鏃堫敂椤撱垺鎯炴繝濠冨姈鐎氳顦伴褍鐏查柛銊﹀▕閺屾洖鈹戦幘瀛樼暥闂佽法鍠嶇划娆撳蓟閸℃瑩鍙￠柨鐕傛嫹?闂佸搫娲ょ€氼垶寮鍫熸櫢缂佹稑顑勭槐锟�??
//OK
//AT+CSCON=1 闂佽法鍣﹂幏锟�//闂佽法鍠撻悺鏂库枔娴犲鏅稿ù鐘差儐缁朵即鏌嶉褌鎲鹃柟顔兼閺佹挾绮电€ｎ偅鍎梺绋挎櫙瀹ュ棗顏�?闂佽法鍠曞Λ鍕矗妤ｅ啯鏅搁柡鍌樺€栫€氾拷?
//OK
//AT+CEREG=1 闂佽法鍣﹂幏锟�//闂佽法鍠撻悺鏂库枔娴犲鏅搁柣锝呮湰琚欓梺绋跨箻濞煎骞忛悜鑺ヮ棃閻熸瑱绲借缂傚啯娼欓幏鎴﹀箯閿燂拷?闂佽法鍠曞Λ鍕矗妤ｅ啯鏅搁柡鍌樺€栫€氾拷?
//OK
//const String  AT_command_5310[6]={
//  "AT\n", //闂佽法鍠曟慨銈夊几閻旂儤妗ㄩ柛鏇ㄥ幗鐎氭洟鏌嶉鐘差€撻柟椋庡厴閺佹捇鏁撻敓锟�?闂佽法鍠庨埀顒傚枑閻濐亪鏌ㄩ悢鍛婄伄闁归鍏橀幏鍐礋閼搁潧顏堕梺璺ㄥ枔閺咁偊顢欒箛娑欐櫢闁哄倶鍊栫€氾拷?  
//  "AT+CIMI\n",
//  "AT+CSQ\n",// 闂傚倸鐗愯闁糕晛鎳橀、锝夊礃椤旀儳甯ｉ梺鍛婂浮濞煎骞忛悜鑺ュ仯濠㈣澹曢崑鎾绘煥閻曞倹瀚�?
//  "AT+CEREG?\n",// 闂佽法鍠撻悺鏃傜博婵犳碍鏅搁柡鍌樺€栫€氬綊鎳曞▎鎾存櫢闁芥ê顦伴埛鏍嚂濞嗘垵顨涢柛婊冨暞鐎氬綊鏌ㄩ悢鍛婄伄閻庢凹鍙冮弫鎾寸鐎ｎ偆顩卞浣风劍閻旑剟骞忛悜鑺ユ櫢闁跨噦鎷�?
//  "AT+CGATT?\n",//闂佽崵濮撮ˉ锟犲焵椤掑嫭鏅搁柟瀛樺笚閸炲鏌ら崫鍕靛創婵炵》鎷�? PS 闂佽法鍠愰弸濠氬礈閸洘鏅搁柡鍌樺€栫€氬綊鏌涢垾铏仱闁逞嶆嫹?
//  "AT+CGATT?\n",//  
//};

const String  AT_command_5310[6]={
  "AT", //闂佽法鍠曟慨銈夊几閻旂儤妗ㄩ柛鏇ㄥ幗鐎氭洟鏌嶉鐘差€撻柟椋庡厴閺佹捇鏁撻敓锟�?闂佽法鍠庨埀顒傚枑閻濐亪鏌ㄩ悢鍛婄伄闁归鍏橀幏鍐礋閼搁潧顏堕梺璺ㄥ枔閺咁偊顢欒箛娑欐櫢闁哄倶鍊栫€氾拷?  
  "AT+CIMI",
  "AT+CSQ",// 闂傚倸鐗愯闁糕晛鎳橀、锝夊礃椤旀儳甯ｉ梺鍛婂浮濞煎骞忛悜鑺ュ仯濠㈣澹曢崑鎾绘煥閻曞倹瀚�?
  "AT+CEREG?",// 闂佽法鍠撻悺鏃傜博婵犳碍鏅搁柡鍌樺€栫€氬綊鎳曞▎鎾存櫢闁芥ê顦伴埛鏍嚂濞嗘垵顨涢柛婊冨暞鐎氬綊鏌ㄩ悢鍛婄伄閻庢凹鍙冮弫鎾寸鐎ｎ偆顩卞浣风劍閻旑剟骞忛悜鑺ユ櫢闁跨噦鎷�?
  "AT+CGATT?",//闂佽崵濮撮ˉ锟犲焵椤掑嫭鏅搁柟瀛樺笚閸炲鏌ら崫鍕靛創婵炵》鎷�? PS 闂佽法鍠愰弸濠氬礈閸洘鏅搁柡鍌樺€栫€氬綊鏌涢垾铏仱闁逞嶆嫹?
};


bool  setup_M5310()
{ NB_5310_RES  res;    
  DisMessage(0,"INIT M5310 AT",dc_white); //闂佽法鍠曟慨銈呪枖閻旂厧绀傜憸婵嬪箯閻戝鈧倿顢氱亸浣镐壕缂傚啫鐡ㄩ弸濠氬箯閻戣姤鎲规慨妯诲墯閺€銊︺亜閻戝洦瀚�     
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
  // 闂備胶鏁诲褔宕甸柆宥呯鐟滄繈骞忛柨瀣洸闁靛牆瀛╃粻顖炴⒒閸及鎺楀春閸涙惌鏁氶柛鎰ㄦ櫆鐎电姷绱掗悩鑼闁汇倝绠栭弫鎾剁驳鐎ｃ劉鍋撻幋锔芥櫢闁哄倶鍊栫€氾拷? 0~31闂佹悶鍊栭〃鎰渻瀹€鍐х泊闁活剛澧楁刊钘夆枍閿燂拷 CSQ闂佽法鍠曟慨銈囨閻樺磭鈻曢悗锝庡枛閻掞拷?12闂佽法鍣﹂幏锟�?闂傚倷绶￠崑鎾存櫠濠婂牊鎲樼痪顓炴噺缁躲儲銇勯敐搴″伎闁归鍏橀弫鎾诲磹閻旈姣�?缂傚啠妾ч崑鎾绘煣閿旂晫鎳曢柟鍑ゆ嫹?
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
   
  res = wait_rx_bc(AT_command_5310[3],4000,"OK");  //  NB闂佽法鍠愰弸濠氬礈閸洘鏅搁柡鍌樺€栫€氬綊鏌涢垾铏仱闁逞嶆嫹? +CEREG:0,1 (1-5)
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
  res = wait_rx_bc(AT_command_5310[4],4000,"OK");  //  AT+CGATT?\n",//闂佽崵濮撮ˉ锟犲焵椤掑嫭鏅搁柟瀛樺笚閸炲鏌ら崫鍕靛創婵炵》鎷�? PS 闂佽法鍠愰弸濠氬礈閸洘鏅搁柡鍌樺€栫€氬綊鏌涢垾铏仱闁逞嶆嫹?  :1 ok  0:false    
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


// 闂佽崵濮撮幊澶愬垂瑜版帗鏅搁柨鐕傛嫹 闂傚倸鐗婅摫濠㈢櫢鎷�?闂佸搫娴勯幏锟�?濡炲绁撮弲顏堝箯閻戣棄绠婚弶鍫氭櫇濞奸亶鏌ㄩ悢鍛婄伄闁瑰嚖鎷�?闂佽法鍠愰弸璇测枔椤掑嫭鏅搁柡鍌樺€栫€氬綊鎮归悜鍡樼窙闁归鍏樺Λ鍐潨閸℃瑦娈梺璺ㄥ枍缁瑦鍒婇崼鏇礊妞ゆ牗绋掑Ч鐐烘煥閻斿憡鐏柛鎾舵暬閺屽矂鏁愯箛鏂款伓濡炲澧介妴瀣箯閿燂拷 闂傚倸顑戦幏锟�?闂佽法鍠愰弸濠氬箰閵堝憘鏃傚枈婢跺顏�? 闂佽法鍠愰弸璇差渻閸儲鐓傞柨婵嗙箲鐎氳顦伴崜褋鈧骞忛敓锟� 闂傚倸顑戦幏锟�?闂佽法鍠愰弸濠氬礄閻樼粯鏅稿ù鐘差儐閸撴垿鎮介悮瀛樺??

const String  AT_MIPLADDOBJ[4]={//闂佽法鍠撻悺鏃堟偩椤撱垺鐒奸柨婵嗘閸も偓闁伙絿娅㈢紞鍥箯閿燂拷  // 闂傚倸娲ら悧濠傗枖閻斿吋瀚婃い鎾跺У鐎氭洟鏌嶉鐘差€撻柟椋庡厴濡啫螣鐠囧樊妫�?闂佸搫娴勯幏锟�?濡炲妯婃禍顏堝吹閺囥垺鏅搁柡鍌樺€栭ˇ顕€鏌ㄩ悤鍌涘
  "AT+MIPLCREATE=49,130031F10003F2002304001100000000000010123138332E3233302E34302E33393A35363833000131F300080000000000,0,49,0 ",  
  "AT+MIPLADDOBJ=0,3200,1,\"1\",0,1 ",   //闂佸湱鍋撻悧鏇㈩敋閺嶎厼绠氶柨鐕傛嫹   闂傚倸鐗婇敃顐﹀汲婢舵劖鏅稿ù鐘差儏椤拷?闂佹悶鍊栭〃鎰渻閸℃稒鏅稿ù鐘差儐缁犳垿鏌ㄥ┑鍛瓘闁瑰嚖鎷� Object 3200闂佹悶鍊栭〃鎰板几閻摴tance 0 闂備礁鐏氱换鍕偩閸洘鏅搁柨鐕傛嫹
  "AT+MIPLDISCOVERRSP=0,3200,1,4,\"5750\"",   //闂傚倸娲ら悧鐐靛緤瑜版帗鏅搁柨鐕傛嫹 Resource闂傚倸锕ゆ蹇涘礈閻㈠憡鏅搁柨鐕傛嫹 闂傚倸鐗婇敃顐﹀汲婢舵劖鏅稿ù鐘差儏椤拷?闂傚倸鎳愰崕鎴﹀疮閿燂拷?闂佹寧绻€缁躲倝骞忛敓锟� Object3200闂佹悶鍊栭〃鎰板几閻摴tance 0 闂佽法鍣﹂幏锟�? 5750 Resource 闂備礁鐏氱换鍕偩閸洘鏅搁柨鐕傛嫹
  "AT+MIPLOPEN=0,3000,30" ,     //闂佽法鍠嶇划娆撍囬妸鈺傛櫢闁兼亽鍎茬€殿參鏌涢悩鍐插鐎靛府鎷�    
};
bool  MIPLADDOBJ()
{  NB_5310_RES  res;   
   res=wait_rx_bc(AT_MIPLADDOBJ[0],3000,"+MIPLCREATE:0"); //  //闂佽法鍠撻悺鏃堟偩椤撱垺鐒奸柨婵嗘閸も偓闁伙絿娅㈢紞鍥箯閿燂拷 闂傚倸娲ら悧濠傗枖閻斿吋瀚婃い鎾跺У鐎氭洟鏌嶉鐘差€撻柟椋庡厴濡啫螣鐠囧樊妫�?闂佸搫娴勯幏锟�?濡炲妯婃禍顏堝吹閺囥垺鏅搁柡鍌樺€栭ˇ顕€鏌ㄩ悤鍌涘 
    if(res.status) {
    DisMessage(1,"MIPLCREATE OK:"+res.temp,dc_gre);
    }
        else{                        
    DisMessage(1,"MIPLCREATE false闂佹悶鍊栭〃鎰板汲閵嗩檴OBJ quit!",dc_red);   
    return (false);
      } 
    delay(2000);

   res=wait_rx_bc(AT_MIPLADDOBJ[1],3000,"OK"); //闂傚倸娲ら悧鐐靛緤瑜版帗鏅搁柨鐕傛嫹 Object 闂佸湱鍋撻悧鏇㈩敋閺嶎厼绠氶柨鐕傛嫹
    if(res.status) {
    DisMessage(1,"MMIPLADDOBJ OK:"+res.temp,dc_gre);
    }
        else{                        
    DisMessage(1,"ADDOBJ false闂佹悶鍊栭〃鎰板汲閵嗩檴OBJ quit!",dc_red);   
    return (false);
      } 
    delay(2000);    

   res=wait_rx_bc(AT_MIPLADDOBJ[2],3000,"OK"); //闂傚倸娲ら悧鐐靛緤瑜版帗鏅搁柨鐕傛嫹 Resource闂傚倸锕ゆ蹇涘礈閻㈠憡鏅搁柨鐕傛嫹   
    if(res.status) {
    DisMessage(1,"MIPLDISCOVERRSP OK:"+res.temp,dc_gre);
    }  
        else{                        
    DisMessage(1,"DISCOVERRSP false闂佹悶鍊栭〃鎰板汲閵嗩檴OBJ quit!",dc_red);   
    return (false);
      } 
    delay(2000);
      
   res=wait_rx_bc(AT_MIPLADDOBJ[3],3000,"OK"); ////闂佽法鍠嶇划娆撍囬妸鈺傛櫢闁兼亽鍎茬€殿參鏌涢悩鍐插鐎垫澘绻樺畷鐢告晸閿燂拷?3000 闂傚倸鎳愰崕鎴﹀箯閻戣棄鐏虫繝鍨尰鐎氾拷?闂佹眹鍨归悧鍐╂叏閵堝鐓傞柨婵嗙箲鐎氳顦伴崣銉т覆闁归鍏橀弫鎾剁驳鐎ｎ剦浼堥梺璺ㄥ枑閺嬪骞忛悜鏂ょ礈闁硅櫣鍋炵€氬綊鏌ｉ敂鑲╃М妞わ富浜炲姗€顢旀惔銏狀伓? 闂佹拝闄勯敃鈺佄ｉ弽顓熸櫢闁跨噦鎷�30闂傚倸鎳愰崕鎴﹀箯閻戣棄鐏虫繝鍨尰鐎氱懓顪冩担绋挎灕闁归鍏樺Λ鍡涙倻閸パ勬瘒闁活偅顑欐禍鐑藉箯閻戣棄鎹堕柟鐑樼箰缁憋拷 闂佹拝闄勯敃鈺佄ｉ弽顓熸櫢闁跨噦鎷�
    if(res.status) {
    DisMessage(1,"MIPLOPEN OK:"+res.temp,dc_gre);    
    }  
        else{                        
    DisMessage(1,"MIPLOPEN false闂佹悶鍊栭〃鎰板汲閵嗩檴OBJ quit!",dc_red);   
    return (false);
      } 
    delay(2000);  
  }

  /*
闂佽崵濮撮幊澶愬垂瑜版帗鏅搁柡鍌樺€栫€氱懓顪冩担绋挎灕闁归鍏橀弫鎾剁驳鐎ｎ偅鍎ч梺璺ㄥ枑閺嬪骞忛悜绛嬫晽闁绘垵娲︾€氬綊鏌熼崜褍鏋庨柡灞界Ч椤€垺绺介崨濠庡晣闁活剟鍋婇崹浠嬪箯閻戣姤鏅搁柨鐕傛嫹? OBSERVE 闂佷紮瀵岄悡澶婎啅閼测晠鍙￠柛婊冨暞鐎氾拷? DISCOVER 闂佷紮瀵岄悡澶婎啅閼测晠鍙￠柛娑卞幗椤斿倿鎮介崫鍕攭濠碘剝妞介幗婵婄疀閵夈儱顥掑銈庝悍閹凤拷
闂佽法鍠曞Λ鍕疾娴犲鏅稿ù鐘差儏椤盯鏌￠崒娑辨闁活厺绶氶幆渚€鈥﹂幒鎴ｅУ缂傚啯娼欓幊妯讳繆濞差亷绱叉繝鍨尰鐎氬綊鏌涢妶鍡╂婵＄偛妫濋弫鎾诲棘閵堝棗顏�?闂佽法鍠曞Λ鍕触閳э拷?濡炲鍋呭Λ鎴﹀箯閻戣棄鍚规俊銈咁儐椤︻參鏌ㄩ悢鍛婄伄闁归鏌夋禒姘舵煥閻旂粯顥夐柛鐘愁殜閿涙柨顫滈崼鐔奉伓闁荤喐顨愰幏锟�?濡炲鎷濋幏锟�?濡炲鍋呭Λ鎴﹀箯閻戣姤鏅搁柨鐕傛嫹?
[#Recv] +MIPLOBSERVE:0,39121,1,3200,0,-1
[#Recv] +MIPLDISCOVER:0,39122,3200
[#Recv] +MIPLREAD:0,39123,3,0,-1,0
闂佽法鍣﹂幏锟�? 闂侀€涚矙閸嬪﹪鎳楅崱娑欐櫢闁兼亽鍎遍弳銏☆槹椤愶絾顥栭柟椋庡厴閺屾洟鏁撻敓锟�?闂佹悶鍊栭〃鎰渻閸℃稒鏅搁柡鍌樺€栫€氬湱鎳庨崼銏犲壋鐎规洖锕弫鎾绘晸閿燂拷?+MIPLEVENT:0,6 闂佷紮瀵岄悡澶婎啅閼测晠鍙￠柨鐕傛嫹 闂佽法鍠愰崹婵堚偓瑙勫▕閺佸秹宕卞☉娆戠暠闂佸搫鍊归〃澶嬩繆濞差亝鎲归煫鍥ュ劚婢瑰秹鎯夐埀顒勫焵椤掑嫭鏅搁柛濠勫枂閳ь剙鍊婚悗楣冨煛閸屾稒顔�?
OneNET 闂佺懓澧介崰搴ㄥ蓟瀹ュ鏁樼痪顓㈡敱鐎氳銇勯垾鍐插毐闁瑰嚖鎷� 闂佹悶鍊ч幏锟�? 闂佺懓澧介崰搴∥涢妷鈺傛櫢闁芥ê顦伴锟�? 闂佽法鍠栭幗顏堝焵椤掑嫭鈷戞い鎺戭槸閸炪劑鏌ㄩ悢璇残撻柡鍡╁亰閿涘秹骞忕仦绛嬬€遍梺鎾呰閹凤拷? 闂佽法鍠愰崹婵堣姳閻戣姤鏅搁柡鍌樺€栫€氾拷? 闂佽法鍣﹂幏锟�? OBSERVE 闂佷紮瀵岄悡澶婎啅閼测晠鍙￠柛婊冨暞鐎氾拷? 闂佽法鍣﹂幏锟�? DISCOVER
闂佷紮瀵岄悡澶婎啅閼测晠鍙￠柨鐕傛嫹 闂佽法鍣﹂幏锟�? 闂佹悶鍊栭〃鎰渻閸垺妗ㄩ柟鎷屽煐鐎氾拷?闂佽法鍠曟慨銈嗙珶閿燂拷?濡炲鍋撳鍧楁倶濞嗘挻鏅搁柨鐕傛嫹? 闂佽法鍠曞Λ鍕潖妤ｅ啯鏅搁柨鐕傛嫹 闂佽法鍠愰弸璇测枔閳哄懏鏅搁柨鐕傛嫹
*/

const String  AT_MIPLNOTIFY[4]={                    // OneNET 闂佽法鍠曞Λ鍕极閻㈠壊妲柣娆忔噺鐎氬綊鏌涢垾铏珔闁归鍏橀幆浣割潩閻撳骸鏁ら梺璺ㄥ櫐閹凤拷
"AT+MIPLNOTIFY=0,0,3200,0,5750,1,4,\"177788\",0,0,12", //闂佹悶鍊栭〃鎰渻閸ヮ剚瀚冨ǎ鍥ㄧ煘kid闂佹悶鍊栭〃鎰板几閻曠穯id闂佷紮璐熼崕娲箚閿燂拷?闂佹剚鍘搁弲顏堝箯閻戣姤鏅搁柟瀛樺笂娴滐拷?缂傚啫鐡ㄩ幏婵囨叏娴煎瓨鏅哥紒娑橆儔閸欏嫰鏌ㄩ悢鍓佺煓濠殿喗鐗犻敍姘瑹婵犲啫顎忛梺璺ㄥ枙濡嫰寮悽鍓叉К闁跨噦鎷�   
"AT+MIPLREAD=0,60204,3200,0,5505,\"E309C82FE6\",1",     //Read 闂佽法鍠愰弸璇测枔閳哄懏鏅搁柡鍌樺€栫€氬綊鎮鹃惂鍛婂?濡炰椒鐒﹂敃顐﹀箯閻戣姤鎲归柨鐕傛嫹?
"AT+MIPLWRITE=0,62069,1",                             //Write 闂佽法鍠愰弸璇测枔閳哄懏鏅搁柡鍌樺€栫€氬綊鎮鹃惂鍛婂?濡炰椒鐒﹂敃顐﹀箯閻戣姤鎲归柨鐕傛嫹?
"AT+MIPLEXECUTE=0,46081,1",                           //Execute 闂佽法鍠愰弸璇测枔閳哄懏鏅搁柡鍌樺€栫€氬綊鎮鹃惂鍛婂?濡炰椒鐒﹂敃顐﹀箯閻戣姤鎲归柨鐕傛嫹?    
};

const String  AT_MIPLCLOSE[3]={                   // OneNET 闂佽法鍠嶇划娆撍囬妸鈺傛櫢闁伙絽鏈闂佸憡甯″濠氬箯閻戣姤鍎婃慨妯煎帶閸炪劑鏌ㄩ悤鍌涘
"AT+MIPLCLOSE=0",           //闂佽法鍠嶇划娆撍囬妸鈺傛櫢闁伙絽鏈闂佸憡甯″濠氬箯閻戣姤鍎婃慨妯煎帶閸炪劑鏌ㄩ悤鍌涘
"AT+MIPLDELOBJ=0,3200,0",   //闂佽崵濮撮幊澶愬垂瑜版帗鏅稿ù鐘差儐閸撴垿鏌熺喊澶嬪?闂佹寧绻€缁躲倝骞忛悜鑺ヮ棅闊洢鍎辨竟宥夋煥閻斿憡鐏柟鐑筋棑閻庡搫鈹冮柨顖氫壕婵犲灚鎸荤€氳顦版潪鎵窗婵犫偓閿燂拷
"AT+MIPLDEL=0",             //闂佽崵濮撮幊澶愬垂瑜版帗鏅稿ù鐘差儐閸撴垿鏌熼懜纰樻＇闁逞屽墰閻忕娀顢涘⿰鍛噭闂侀€涚矙閸嬪﹪骞忛崨瀛樻櫢闁哄倶鍊栫€氬湱鎳庨崼娑樼労闁愁垽鎷�  
};


// 闂佽法鍠撻悺鏃堝汲閻愭湹绮撮悷顓炵墢閻★拷 闂佽法鍠愰弸鑽も偓姘€鍛殧婵☆垳鍎ら鍌涱槹閸撗佲偓瀣箯閻戣棄鏋侀柣姘綑閺嗩偅銇勫☉娆愵棖闁圭兘顥撻悗鍝勨枎濞嗘劕顏堕梺鐟版▕閸欏繐鈻撹ぐ鎺撴櫢濞寸姴顑愬Σ鑸点亜椤愩垹浜伴柟椋庡厴閹芥繆绠涢敐鍡╁晭闂佽法鍠嶇划娆徝归敓锟�? NB_5310_RES
//struct NB_5310_RES
//{
//  bool status;  闂佽法鍠撻悺鏃€绋夐敓锟�?濡炰椒鐒﹂敃顐﹀箯閻戣姤鏅搁柨鐕傛嫹?
//  String data;  婵＄偘绀侀悧婊冾焽閸涘瓨鏅搁柡鍌樺€栫€氳顦伴锝呴嚋闁归鍏橀幗婵嗩潩閾忣偆褰ч梺鎴掔筏閹凤拷
//  String temp;  闂備礁鐏氱换鍐不閸撲焦绶查柨鐕傛嫹 
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
    M5310_Serial.println(str_ATcmd);   //闂佽法鍠撻悺鏃堝汲閻愭湹绮撮悷顓炵墢閻℃鏌ㄩ悢灏佸亾閻旂ǹ巍闁荤喐鎷濋幏锟�
    DisMessage(0,"SEND Com="+str_ATcmd,dc_white); //闂佽法鍠曟慨銈呪枖閻旂厧绀傜憸婵嬪箯閻戝鈧倿顢氱亸浣镐壕缂傚啫鐡ㄩ弸濠氬箯缁屽獪mmand 
    if( M5310_Serial.available())   //闂備礁鐏氱换鍡涘礉婵犲浂鏁樻い鏍ュ€栫€氳顦伴崙銈囩Ф闁归鍏橀獮澶愭晸閿燂拷?
    {
      input = M5310_Serial.readStringUntil('\n');      
      res_.temp+=input;      
      if(input.indexOf(str_wait)!=-1)//闂佹眹鍨瑰ù鍕箯閻戣姤鏅柛鈽嗗幗鐎氬綊鏌涢垾铏珔鐎规洩鎷�
      {
        res= true;
        res_.status = res;
        res_.data = input;   
        DisMessage(3,str_ATcmd+"---"+res_.temp ,dc_white); //闂佹眹鍨瑰ù鍕箯閻戣姤鏅柛鈽嗗幗鐎氬綊鏌涢垾铏珔鐎规洩鎷�       
   
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
      DisMessage(3,"AT OUT time!--" ,dc_red); // 闂傚倸顑囬崕鎴﹀疮閾忚绶查柨鐕傛嫹
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
  
//Mode闂佹悶鍊栭〃蹇涱敂椤斿墽鈧偉绠涘顓犺棭缂備胶濮甸悺鏇熶繆濞差亜绠绘慨妤€妫欓崕娆撴嚂閿燂拷?
//.7 闂佸湱鐡旈崳顕€篓閳э拷?闁荤偞绋堥崑锟�?闁荤姭鏂傞崹娲偪妤ｅ啯顥堟慨妯哄綖缁憋附銇勯澶婃珝婵犫偓閳ь剟鏌￠崶褍顏柡渚婃嫹 1=闂傚倸鐗婇悢顒佸閹剧儵鈧牠鏁撻敓锟� 0=闂備礁鐏氱换鎰枔椤曗偓閺佹捇寮妶鍡楊伓濠殿喗锕╅崕鈺呭焵閿燂拷?
//.6 闂佽法鍠愰弸璇差渻閸垻鈧儳鈹戦幘鏉戭伓闂佸吋鎯岄崑鎺椼€呴幘顔芥喒濡炲娴烽弳锟�?闂佸搫娲ょ€氼垶寮敓锟� 1=闂佽法鍠曟慨銈呪枖閻旂厧绀傞柨鐕傛嫹 0=闂備礁鐏氱换鎰枔椤掑倻鈧偉绠涘顓犺棭缂備胶濮电欢鐢稿焵閿燂拷?
//.5-.4 闂佽法鍣﹂幏锟�?0闂佽法鍣﹂幏锟�?
//.3-.0闂佹悶鍊栭〃蹇涱敂椤撱垺鏅搁柡鍌樺€栫€氬綊鏌ｉ悢鍛婄；缂佺媴缍侀獮姗€鎮埀顒勫箚閵忋倖鏅搁柨鐕傛嫹0x00-0x09闂佹悶鍊栭〃鎰渻閿燂拷?闁诲孩绋撻。顕€骞忛悜钘夌妞ゆ洖鎷嬪Ο鍫ユ煥閻旇袚闁哥喐濞婇獮宥夋儌閸涘﹤顏堕梻浣瑰墯閸欏海鏁Δ鍛櫢闁芥ê顦伴锟�?
//0x00=6*12 0x01=8*16 0x02=10*20 0x03=12*24 0x04=14*28
//0x05=16*32 0x06=20*40 0x07=24*48 0x08=28*56 0x09=32*64
//Color闂佹悶鍊栭〃蹇涱敂椤撱垺鏅搁柛濠勫枑濞撴劙鎮跺☉妯肩劸妞も晜鎸抽幗婵囶槹鎼淬伀锟�?闁伙絽瀵掗崣蹇氥亹閹绢喗鏅搁柨鐕傛嫹?
//Bcolor闂佹悶鍊栭〃蹇涱敂椤撱垺鏅搁柛濠勫枑濞撴劙鎮峰Δ鈧亸鍛村箯閻戣姤鏅搁柨鐕傛嫹?闂佽法鍠曟慨銈呪枖閻旂厧绀傜憸搴ㄥ窗閹邦喗娅楁い鎾卞妼闁伴亶鏌ㄩ悤鍌涘?
//闂佹悶鍊栭〃鎰焊娴兼潙鍚规俊銈咁儐閻撯偓闂佹悶鍊栭〃鎰板箚瀹€鍕櫢闁兼亽鍎查崳娲儔瀹ュ繑瀚�?闂佹儳娴氶崑澶愬箟娴犲鏅搁柤鎭掑劜绾板倿鏌涘鏉跨У鐎氳顦伴锝呯瑲闁靛洦鍔欓弻鏇炩攽閹惧瓨鍎�?濡炪倧绠戦悧鐐哄箯閻戣姤鏅搁柡鍌樺€曢銏ゆ嚂閿燂拷?//string.getBytes(buf, len)
//Strings闂佹悶鍊栭〃蹇涱敂閿燂拷?缂傚啫顕埛鍫ャ€呴幘顔芥喒濡炲娴烽弳蹇涙煥閻旇袚闁瑰啿绻掗悗娲晸閿燂拷?闂佹儳娴氶崑澶愬箟娴犲鍚规俊銈咁儐椤﹂亶鏌ㄩ悢娲暅SCII 闂佸湱鐡旈崳顕€篓閳э拷?闁荤偞绋戦惌澶愬箯閻戣姤鏅搁柨鐕傛嫹?GBK 闂佹拝闄勫鍧楀Υ閹扮増鏅搁柡鍌樺€栫€氳绻濋姀鐘筹紨闁归鍏橀幆浣糕枎濞嗘劕浠奸梺璺ㄥ枑閺嬪骞忛悜钘夊嵆闁告洦鍋勫楣冩煥閻曞倹瀚�?
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


//DIM_Set闂佹悶鍊栭〃蹇涱敂娴煎瓨鏅搁柡鍌樺€栫€氳顦伴弸顐㈢仾缂佽鲸甯￠獮妤呭Ω椤垶鏁ら柤杈ㄧ懃椤洤顫滈崼鐔奉伓0x00-0xFF闂佽法鍣﹂幏锟�?
//0x00 闂佽法鍠愰弸璇差渻閸ヮ剚鏅搁柡鍌樺€栫€氬綊鏌涢悩鍙夆枙闁挎稐绶氬畷鐢告晸閿燂拷?0xFF 闂佽法鍠愰弸璇差渻閸ヮ剚鏅搁柡鍌樺€栫€氬綊鎳曞▎鎾寸厓闁跨噦鎷�?闂佹悶鍊栭〃鎰渻閸ヮ剨绱為柍褜鍓熼弻鏇㈡晸閿燂拷?0x01-0x1F 闂傚倸娲ら悧濠傗枖閻斿吋瀚婃い鎾卞妽鐎氳绻涙總鍛婁氦闁归鍏橀弫鎾绘晸閿燂拷?
//闂佽法鍣﹂幏锟�?闂佽法鍠庨埀顒傚枑婵￠亶鏌ㄩ悢鍛婄伄闁瑰嚖鎷�?闂佽法鍠撻悺鏃堟嚇婵犲懍绮�?
//闂備礁鐏氱换鎰板箹閵娾斁鈧棃骞掗幘鎼殑闁活兘鍋撻柨鐕傛嫹?闂佸搫鍊稿В婵嬪焵椤掆偓椤洭顢樿濞诧拷0xFF闂佽法鍣﹂幏锟�?
//闂備礁鐏氱换鍐ㄢ枖閿曞倹鏅搁柦妯侯槹椤斿倿鎯夐悢鐑樻嚀 30 80 CC 33 C3 3C 闂備胶顒查幏锟�?闂佺懓顫曢崝蹇涘Χ閸ф鏅搁柡鍌樺€栫€氬湱绱樻担鐧稿叕鐎规洩鎷�50%闂佽法鍣﹂幏锟�?
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
