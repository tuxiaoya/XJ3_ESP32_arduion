/*
  String length()

  Examples of how to use length() in a String.
  Open the Serial Monitor and start sending characters to see the results.
  created 1 Aug 2010
  by Tom Igoe

  This example code is 濠碘剝顨嗛、锟? 闁圭儤绋栬ぐ锟? GIT

  move  HWSG to GITHUB
  1-3
  // #include "nbiot_m5310.c"
  http://www.arduino.cc/en/Tutorial/StringLengthTrim
*/
#include <HardwareSerial.h>
#include "DIWEN_480.h"
#include <Ticker.h>
#include "MG_HWSG_2C.H"


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
DIWEN480  MyDIWEN

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
  M5310_Serial.begin(9600);  // 闁跨喎鈧?鐔风到婵°倖绻堟慨鏍?顢″?楀牊瀚归梻鍥х墛閺嬬喖顢犻弮鎾村?规?佷焦鐗?閹凤拷 t
  // DIWEN_Serial.begin(115200, SERIAL_8N1, 17, 16);  //  rxPin, txPin
  HWSG_Serial.begin(1200, SERIAL_8N1, 18, 19);  //  rxPin, txPin
  while (!M5310_Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }  


  if(setup_M5310()) {
    DisMessage(0,"M5310 INIT Succese!",dc_white); 
    DisMessage(1,"New SIGN to ONENET ...........",dc_white);
    if(MIPLADDOBJ()){ // //闁跨喍绮欓ˇ銊╂晸閼恒儲瀵?闁哥姵鍓氬?帮拷
    DisMessage(0,"M5310 ADDOBJ Succese!",dc_white);         
      }    
    }
  else{
    DisMessage(0,"M5310 INIT  falsed!!!",dc_red); 
    DisMessage(1,"-------------",dc_white);    
    }

  ticker_HWSG.attach(1, Call_HWSG);   //闁伙讣鎷??1闁撅附锕╁Σ瀣?鏁撻弬銈嗗?规慨锝呰瘽ll_HWSG  upload tem  
}

void loop() {

}

void DisMessage(char No_place, String msg, uint16_t dcc)
{

  switch (No_place)
  {
  case 0: //
    DisStrings(0x42, dcc, 0x0000, dpx0, 460, msg);
    break;
  case 1: // your hand is close to the sensor
    DisStrings(0x42, dcc, 0x0000, dpx1, 460, msg);
    break;
  case 2: // your hand is close to the sensor
    DisStrings(0x42, dcc, 0x0000, dpx2, 460, msg);
    break;
  case 3: // your hand is close to the sensor
    DisStrings(0x41, dcc, 0x0000, 0, 0, msg);
    break;
  }
}

//const String  AT_command_5310[4]={
//  "AT\r\n", //判断模组是否上电开机成功
//  "AT+CSQ\r\n",// 信号质量检查
//  "AT+CEREG?\r\n",// 域附着状态，标识位返回
//  "AT+CGATT?\r\n",//检查模组 PS 附着状态
//};
//AT+COPS=1,2,"46000" ?//设置手动注册移动运营商 MNC
//OK
//AT+NEARFCN=0,3555 ?//锁定频点为 3555，锁频可以有效减小搜网时间，但是频点设置错误会导致搜网失败，建议通常情况下不要设置锁频
//OK
//AT+CSCON=1 ?//打开信号提示自动上报
//OK
//AT+CEREG=1 ?//打开注册信息自动上报
//OK
//const String  AT_command_5310[6]={
//  "AT\n", //判断模组是否上电开机成功
//  "AT+CIMI\n",
//  "AT+CSQ\n",// 信号质量检查
//  "AT+CEREG?\n",// 域附着状态，标识位返回
//  "AT+CGATT?\n",//检查模组 PS 附着状态
//  "AT+CGATT?\n",//
//};

const String AT_command_5310[6] = {
    "AT", //判断模组是否上电开机成功
    "AT+CIMI",
    "AT+CSQ",    // 信号质量检查
    "AT+CEREG?", // 域附着状态，标识位返回
    "AT+CGATT?", //检查模组 PS 附着状态
};

bool setup_M5310()
{
  NB_5310_RES res;
  DisMessage(0, "INIT M5310 AT", dc_white); //显示发送的符号
  delay(2000);
  res = wait_rx_bc(AT_command_5310[0], 4000, "OK"); //  AT  --  OK
  if (res.status)
  {
    DisMessage(1, "M5310 POWER OK", dc_gre);
  }
  else
  {
    DisMessage(1, "M5310 POWER ERROR! SETUP quit!", dc_red);
    return (false);
  }
  delay(2000);

  delay(2000);
  // 表示接收信号强度取范围 0~31，通常 CSQ值大于12可视为网络能够使用
  res = wait_rx_bc(AT_command_5310[2], 4000, "OK"); //   "AT+CSQ\n",  +CSQ:20,99
  if (res.status)
  {
    char ri;
    ri = res.temp.substring(6, 8).toInt();
    if ((ri > 12) && (ri != 99))
    {
      DisMessage(1, "CSQ:" + res.temp.substring(6, 8), dc_gre);
    }
    else
    {
      DisMessage(1, "No CSQ_sign,SETUP quit!" + res.data.substring(6, 8), dc_red);
      return (false);
    }
  }
  else
  {
    DisMessage(1, "CSQ REV ERROR! SETUP quit!", dc_red);
    return (false);
  }
  delay(2000);

  res = wait_rx_bc(AT_command_5310[3], 4000, "OK"); //  NB附着状态 +CEREG:0,1 (1-5)
  if (res.status)
  {
    char ri;
    ri = res.temp.substring(10).toInt();
    if ((ri >= 1) && (ri <= 5))
    {
      DisMessage(1, "NB+CEREG =" + res.temp.substring(10), dc_gre);
    }
    else
    {
      DisMessage(1, "NB+CEREG false,SETUP quit!", dc_red);
      return (false);
    }
  }
  else
  {
    DisMessage(1, "NB+CEREG REV ERROR! SETUP quit!", dc_red);
    return (false);
  }
  delay(2000);
  res = wait_rx_bc(AT_command_5310[4], 4000, "OK"); //  AT+CGATT?\n",//检查模组 PS 附着状态  :1 ok  0:false
  if (res.status)
  {
    if (res.temp.substring(8).toInt() == 1)
    {
      DisMessage(1, "CGATT=" + res.temp.substring(8), dc_gre);
      DisMessage(0, "M5310 NB_IOT OK!!!", dc_gre);
    }
    else
    {
      DisMessage(1, "CGATT false,SETUP quit!", dc_red);
      return (false);
    }
  }
  else
  {
    DisMessage(1, "CGATT ERROR! SETUP quit!", dc_red);
    return (false);
  }
}

// 模组 侧设备创建，目前我司提供封装好的注册码 转换工具 和注册码 转换源供客

const String AT_MIPLADDOBJ[4] = {
    //创建实体  // 设置模组侧设备注册码
    "AT+MIPLCREATE=49,130031F10003F2002304001100000000000010123138332E3233302E34302E33393A35363833000131F300080000000000,0,49,0 ",
    "AT+MIPLADDOBJ=0,3200,1,\"1\",0,1 ",      //对象   例程中，仅订阅 Object 3200，instance 0 为例
    "AT+MIPLDISCOVERRSP=0,3200,1,4,\"5750\"", //订阅 Resource资源 例程中仅订阅 Object3200，instance 0 的 5750 Resource 为例
    "AT+MIPLOPEN=0,3000,30",                  //登录平台
};
bool MIPLADDOBJ()
{
  NB_5310_RES res;
  res = wait_rx_bc(AT_MIPLADDOBJ[0], 3000, "+MIPLCREATE:0"); //  //创建实体 设置模组侧设备注册码
  if (res.status)
  {
    DisMessage(1, "MIPLCREATE OK:" + res.temp, dc_gre);
  }
  else
  {
    DisMessage(1, "MIPLCREATE false，ADDOBJ quit!", dc_red);
    return (false);
  }
  delay(2000);

  res = wait_rx_bc(AT_MIPLADDOBJ[1], 3000, "OK"); //订阅 Object 对象
  if (res.status)
  {
    DisMessage(1, "MMIPLADDOBJ OK:" + res.temp, dc_gre);
  }
  else
  {
    DisMessage(1, "ADDOBJ false，ADDOBJ quit!", dc_red);
    return (false);
  }
  delay(2000);

  res = wait_rx_bc(AT_MIPLADDOBJ[2], 3000, "OK"); //订阅 Resource资源
  if (res.status)
  {
    DisMessage(1, "MIPLDISCOVERRSP OK:" + res.temp, dc_gre);
  }
  else
  {
    DisMessage(1, "DISCOVERRSP false，ADDOBJ quit!", dc_red);
    return (false);
  }
  delay(2000);

  res = wait_rx_bc(AT_MIPLADDOBJ[3], 3000, "OK"); ////登录平台，3000 代表本次注册成功后的存活时间 秒，30代表登录超时时间 秒；
  if (res.status)
  {
    DisMessage(1, "MIPLOPEN OK:" + res.temp, dc_gre);
  }
  else
  {
    DisMessage(1, "MIPLOPEN false，ADDOBJ quit!", dc_red);
    return (false);
  }
  delay(2000);
}

/*
模组登录成功后，平台会下发 OBSERVE 消息和 DISCOVER 消息；模组收
到这两条消息之后，会自动处理，无需用户另行处理。
[#Recv] +MIPLOBSERVE:0,39121,1,3200,0,-1
[#Recv] +MIPLDISCOVER:0,39122,3200
[#Recv] +MIPLREAD:0,39123,3,0,-1,0
在 实际处理中，依靠判断+MIPLEVENT:0,6 消息 来确认模组是否登录
OneNET 平台成功 ； 并且， 需要等待模组 接受到 到 OBSERVE 消息和 和 DISCOVER
消息 后 ，方可执行数据 收发 操作
*/

const String AT_MIPLNOTIFY[4] = {
    // OneNET 数据收发流程
    "AT+MIPLNOTIFY=0,0,3200,0,5750,1,4,\"177788\",0,0,12", //，带ackid，ackid每次上报要自加，上报数据
    "AT+MIPLREAD=0,60204,3200,0,5505,\"E309C82FE6\",1",    //Read 操作回复流程
    "AT+MIPLWRITE=0,62069,1",                              //Write 操作回复流程
    "AT+MIPLEXECUTE=0,46081,1",                            //Execute 操作回复流程
};

const String AT_MIPLCLOSE[3] = {
    // OneNET 登录注销流程
    "AT+MIPLCLOSE=0",         //登录注销流程
    "AT+MIPLDELOBJ=0,3200,0", //模组侧订阅资源列表释放
    "AT+MIPLDEL=0",           //模组侧通信实例删除
};

// 发送AT 指令，等待接受回复，返回结构体 NB_5310_RES
//struct NB_5310_RES
//{
//  bool status;  回复成功
//  String data;  返回的字符串
//  String temp;  临时
//};
#define trytime 3
NB_5310_RES wait_rx_bc(String str_ATcmd, long tout, String str_wait)
{
  unsigned long pv_ok = millis();
  unsigned long current_ok = millis();
  String input;
  unsigned char flag_out = 1;
  NB_5310_RES res_;
  bool res = false;
  res_.temp = "";
  res_.data = "";

  while (M5310_Serial.read() >= 0)
    ;
  while (flag_out < trytime)
  {
    M5310_Serial.println(str_ATcmd);                  //发送AT命令
    DisMessage(0, "SEND Com=" + str_ATcmd, dc_white); //显示发送的command
    if (M5310_Serial.available())                     //串口有回复
    {
      input = M5310_Serial.readStringUntil('\n');
      res_.temp += input;
      if (input.indexOf(str_wait) != -1) //正确收到
      {
        res = true;
        res_.status = res;
        res_.data = input;
        DisMessage(3, str_ATcmd + "---" + res_.temp, dc_white); //正确收到

        return (res_);
      }
      else if (input.indexOf(F("ERROR")) != -1)
      {
        res = false;
        flag_out++;
        res_.temp = "";
        res_.data = "";
      }
    }
    current_ok = millis();
    if (current_ok - pv_ok >= tout)
    {
      // flag_out=5;
      res = false;
      res_.status = res;
      res_.data = "AT OUT time";
      while (M5310_Serial.read() >= 0)
        ;
      DisMessage(3, "AT OUT time!--", dc_red); // 超时
      return (res_);
    }
  }
}

void Call_HWSG()
{
  static bool hh = true;
  HWSG_Serial.write(HWSG_CMD_Upload_tem); // send 0xc0 2 times
  HWSG_Serial.write(HWSG_CMD_Upload_tem);
  hh = !hh;
  if (hh)
  {
    DisMessage(2, "-", dc_red); //
  }
  else
  {
    DisMessage(2, "|", dc_red);
  } //
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

