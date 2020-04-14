/** 
 *  TaskScheduler Test
 *
 */

#include <TaskScheduler.h>
#include <HardwareSerial.h>
#include "DIWEN_480.h"
#include <Ticker.h>
#include "MG_HWSG_2C.H"
#include "M5310A.H"
#include "EasyButton.h"
#include "HWSG_XJ3_PORT.h"

// Callback methods prototypes
void t1_Display_Callback();
void t2_HwsgUart_Callback(); //  every second read  uart HWSG 2C
void t3_NBIOT5310_Callback();       //   rvery sec send NB_IOT
void t4_button_Callback();          //    

//Tasks  4大任务
Task XJ3_button(); // 键盘任务
Task NBIOT5310(2000, 10, &t3_NBIOT5310_Callback);
Task TaskHwsgUart(1000, TASK_FOREVER, &t2_HwsgUart_Callback);
Task TaskDisplay(5000, TASK_FOREVER, &t1_Display_Callback);

// 初始化系统对象：任务调度Scheduler     M5310A，DIWEN HWSG2C 0#
Scheduler HWSG_IOT_run;
NB_M5310A M5310A(0);                 // NB_M5310A  HardwareSerial 0
DIWEN480  DIWEN854480(1);              //  HardwareSerial uart Number 1
IR_Sensor_HWSG2C_Online HWSG2C_0(0, HWSG_TYPE_HIGHTEM ,2); // HWSG ID 0, HardwareSerial uart Number 2


// 按键 define
EasyButton UP_button(KEY_UP);
EasyButton DOWN_button(KEY_DOWN);
EasyButton LEFT_button(KEY_LEFT);
EasyButton RIGHT_button(KEY_RIGHT);
EasyButton SELECT_button(KEY_SELECT);

// 按键处理 Callback function  
void Up_ButtonPressed()
{
    DIWEN854480.DisMessage(0, "Up_ButtonPressed!", dc_white);
}
void DOWN_ButtonPressed()
{
    DIWEN854480.DisMessage(0, "DOWN_ButtonPressed!", dc_white);
}
void LEFT_ButtonPressed()
{
    DIWEN854480.DisMessage(0, "LEFT_ButtonPressed!", dc_white);
}
void RIGHT_ButtonPressed()
{
    DIWEN854480.DisMessage(0, "RIGHT_ButtonPressed!", dc_white);
}
void SELECT_ButtonPressed()
{
    DIWEN854480.DisMessage(0, "SELECT_ButtonPressed!", dc_white);
 }

void sys_init()
{   DIWEN854480.Begin();    
    M5310A.Begin(); 
    HWSG2C_0.Begin();
    
    // 按钮处理程序
    UP_button.begin();
    UP_button.onPressed(Up_ButtonPressed);
    DOWN_button.begin();
    DOWN_button.onPressed(DOWN_ButtonPressed);
    LEFT_button.begin();
    LEFT_button.onPressed(LEFT_ButtonPressed);
    RIGHT_button.begin();
    RIGHT_button.onPressed(RIGHT_ButtonPressed);
    SELECT_button.begin();
    SELECT_button.onPressed(SELECT_ButtonPressed);


     
}

    void setup()
    {
        sys_init();
    }
    void loop()
    {
        HWSG_IOT_run.execute();
    }




    void t1_Display_Callback()
    {
    }

    void t2_HwsgUart_Callback()
    {
 
    }

    void t3_NBIOT5310_Callback()
    {
     
    }

    void t4_button_Callback()
    {
    }
