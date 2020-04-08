/*
  String length()

  Examples of how to use length() in a String.
  Open the Serial Monitor and start sending characters to see the results.
  created 1 Aug 2010
  by Tom Igoe

  This example code is 婵℃顢�? 閹烘稖褰�? GIT

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
  M5310_Serial.begin(9600);  // 閿熷�?熷絽濡ゆ繈濮�?�?�堟嫹闂囧牊鏋熼鏃撴�?��?�佹�?鎷� t
  // DIWEN_Serial.begin(115200, SERIAL_8N1, 17, 16);  //  rxPin, txPin
  HWSG_Serial.begin(1200, SERIAL_8N1, 18, 19);  //  rxPin, txPin
  while (!M5310_Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }  


  if(setup_M5310()) {
    DisMessage(0,"M5310 INIT Succese!",dc_white); 
    DisMessage(1,"New SIGN to ONENET ...........",dc_white);
    if(MIPLADDOBJ()){ // //閿熶粙顦ㄩ敓鑺ユ�?閸犳剚�?��
    DisMessage(0,"M5310 ADDOBJ Succese!",dc_white);         
      }    
    }
  else{
    DisMessage(0,"M5310 INIT  falsed!!!",dc_red); 
    DisMessage(1,"-------------",dc_white);    
    }

  ticker_HWSG.attach(1, Call_HWSG);   //閻︼�??1閾︽﹩妲�?敓鏂ゆ�?�濮ｅ话ll_HWSG  upload tem  
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
//  "AT\r\n", //�ж�ģ���Ƿ��ϵ翪���ɹ�
//  "AT+CSQ\r\n",// �ź��������
//  "AT+CEREG?\r\n",// ����״̬����ʶλ����
//  "AT+CGATT?\r\n",//���ģ�� PS ����״̬
//};
//AT+COPS=1,2,"46000" ?//�����ֶ�ע���ƶ���Ӫ�� MNC
//OK
//AT+NEARFCN=0,3555 ?//����Ƶ��Ϊ 3555����Ƶ������Ч��С����ʱ�䣬����Ƶ�����ô���ᵼ������ʧ�ܣ�����ͨ������²�Ҫ������Ƶ
//OK
//AT+CSCON=1 ?//���ź���ʾ�Զ��ϱ�
//OK
//AT+CEREG=1 ?//��ע����Ϣ�Զ��ϱ�
//OK
//const String  AT_command_5310[6]={
//  "AT\n", //�ж�ģ���Ƿ��ϵ翪���ɹ�
//  "AT+CIMI\n",
//  "AT+CSQ\n",// �ź��������
//  "AT+CEREG?\n",// ����״̬����ʶλ����
//  "AT+CGATT?\n",//���ģ�� PS ����״̬
//  "AT+CGATT?\n",//
//};

const String AT_command_5310[6] = {
    "AT", //�ж�ģ���Ƿ��ϵ翪���ɹ�
    "AT+CIMI",
    "AT+CSQ",    // �ź��������
    "AT+CEREG?", // ����״̬����ʶλ����
    "AT+CGATT?", //���ģ�� PS ����״̬
};

bool setup_M5310()
{
  NB_5310_RES res;
  DisMessage(0, "INIT M5310 AT", dc_white); //��ʾ���͵ķ���
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
  // ��ʾ�����ź�ǿ��ȡ��Χ 0~31��ͨ�� CSQֵ����12����Ϊ�����ܹ�ʹ��
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

  res = wait_rx_bc(AT_command_5310[3], 4000, "OK"); //  NB����״̬ +CEREG:0,1 (1-5)
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
  res = wait_rx_bc(AT_command_5310[4], 4000, "OK"); //  AT+CGATT?\n",//���ģ�� PS ����״̬  :1 ok  0:false
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

// ģ�� ���豸������Ŀǰ��˾�ṩ��װ�õ�ע���� ת������ ��ע���� ת��Դ����

const String AT_MIPLADDOBJ[4] = {
    //����ʵ��  // ����ģ����豸ע����
    "AT+MIPLCREATE=49,130031F10003F2002304001100000000000010123138332E3233302E34302E33393A35363833000131F300080000000000,0,49,0 ",
    "AT+MIPLADDOBJ=0,3200,1,\"1\",0,1 ",      //����   �����У������� Object 3200��instance 0 Ϊ��
    "AT+MIPLDISCOVERRSP=0,3200,1,4,\"5750\"", //���� Resource��Դ �����н����� Object3200��instance 0 �� 5750 Resource Ϊ��
    "AT+MIPLOPEN=0,3000,30",                  //��¼ƽ̨
};
bool MIPLADDOBJ()
{
  NB_5310_RES res;
  res = wait_rx_bc(AT_MIPLADDOBJ[0], 3000, "+MIPLCREATE:0"); //  //����ʵ�� ����ģ����豸ע����
  if (res.status)
  {
    DisMessage(1, "MIPLCREATE OK:" + res.temp, dc_gre);
  }
  else
  {
    DisMessage(1, "MIPLCREATE false��ADDOBJ quit!", dc_red);
    return (false);
  }
  delay(2000);

  res = wait_rx_bc(AT_MIPLADDOBJ[1], 3000, "OK"); //���� Object ����
  if (res.status)
  {
    DisMessage(1, "MMIPLADDOBJ OK:" + res.temp, dc_gre);
  }
  else
  {
    DisMessage(1, "ADDOBJ false��ADDOBJ quit!", dc_red);
    return (false);
  }
  delay(2000);

  res = wait_rx_bc(AT_MIPLADDOBJ[2], 3000, "OK"); //���� Resource��Դ
  if (res.status)
  {
    DisMessage(1, "MIPLDISCOVERRSP OK:" + res.temp, dc_gre);
  }
  else
  {
    DisMessage(1, "DISCOVERRSP false��ADDOBJ quit!", dc_red);
    return (false);
  }
  delay(2000);

  res = wait_rx_bc(AT_MIPLADDOBJ[3], 3000, "OK"); ////��¼ƽ̨��3000 ������ע��ɹ���Ĵ��ʱ�� �룬30�����¼��ʱʱ�� �룻
  if (res.status)
  {
    DisMessage(1, "MIPLOPEN OK:" + res.temp, dc_gre);
  }
  else
  {
    DisMessage(1, "MIPLOPEN false��ADDOBJ quit!", dc_red);
    return (false);
  }
  delay(2000);
}

/*
ģ���¼�ɹ���ƽ̨���·� OBSERVE ��Ϣ�� DISCOVER ��Ϣ��ģ����
����������Ϣ֮�󣬻��Զ����������û����д���
[#Recv] +MIPLOBSERVE:0,39121,1,3200,0,-1
[#Recv] +MIPLDISCOVER:0,39122,3200
[#Recv] +MIPLREAD:0,39123,3,0,-1,0
�� ʵ�ʴ����У������ж�+MIPLEVENT:0,6 ��Ϣ ��ȷ��ģ���Ƿ��¼
OneNET ƽ̨�ɹ� �� ���ң� ��Ҫ�ȴ�ģ�� ���ܵ� �� OBSERVE ��Ϣ�� �� DISCOVER
��Ϣ �� ������ִ������ �շ� ����
*/

const String AT_MIPLNOTIFY[4] = {
    // OneNET �����շ�����
    "AT+MIPLNOTIFY=0,0,3200,0,5750,1,4,\"177788\",0,0,12", //����ackid��ackidÿ���ϱ�Ҫ�Լӣ��ϱ�����
    "AT+MIPLREAD=0,60204,3200,0,5505,\"E309C82FE6\",1",    //Read �����ظ�����
    "AT+MIPLWRITE=0,62069,1",                              //Write �����ظ�����
    "AT+MIPLEXECUTE=0,46081,1",                            //Execute �����ظ�����
};

const String AT_MIPLCLOSE[3] = {
    // OneNET ��¼ע������
    "AT+MIPLCLOSE=0",         //��¼ע������
    "AT+MIPLDELOBJ=0,3200,0", //ģ��ඩ����Դ�б��ͷ�
    "AT+MIPLDEL=0",           //ģ���ͨ��ʵ��ɾ��
};

// ����AT ָ��ȴ����ܻظ������ؽṹ�� NB_5310_RES
//struct NB_5310_RES
//{
//  bool status;  �ظ��ɹ�
//  String data;  ���ص��ַ���
//  String temp;  ��ʱ
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
    M5310_Serial.println(str_ATcmd);                  //����AT����
    DisMessage(0, "SEND Com=" + str_ATcmd, dc_white); //��ʾ���͵�command
    if (M5310_Serial.available())                     //�����лظ�
    {
      input = M5310_Serial.readStringUntil('\n');
      res_.temp += input;
      if (input.indexOf(str_wait) != -1) //��ȷ�յ�
      {
        res = true;
        res_.status = res;
        res_.data = input;
        DisMessage(3, str_ATcmd + "---" + res_.temp, dc_white); //��ȷ�յ�

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
      DisMessage(3, "AT OUT time!--", dc_red); // ��ʱ
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

