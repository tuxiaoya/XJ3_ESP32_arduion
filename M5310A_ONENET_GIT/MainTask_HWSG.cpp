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

// Callback methods prototypes
void t1_Display_Callback();
void t2_HwsgUart_Callback(); //  every second read  uart HWSG 2C
void t3_NBIOT5310_Callback();       //   rvery sec send NB_IOT
void t4_keyBoard_Callback();        //   rvery sec send NB_IOT

//Tasks  4大任务  
Task keyBoard();    // 键盘任务
Task NBIOT5310(2000, 10, &t3_NBIOT5310_Callback);
Task TaskHwsgUart(1000, TASK_FOREVER, &t2_HwsgUart_Callback);
Task TaskDisplay(5000, TASK_FOREVER, &t1_Display_Callback);

// 初始化系统对象：任务调度Scheduler     M5310A，DIWEN HWSG2C 0#
Scheduler HWSG_IOT_run;
NB_M5310A M5310A(0);                 // NB_M5310A  HardwareSerial 0
DIWEN480  DIWEN854480(1);              //  HardwareSerial uart Number 1
IR_Sensor_HWSG2C_Online HWSG2C_0(0, HWSG_TYPE_HIGHTEM ,2); // HWSG ID 0, HardwareSerial uart Number 2

void sys_init()
{   DIWEN854480.Begin();    
    M5310A.Begin(); 
    HWSG2C_0.Begin(); 
}

    void setup()
    {
        sys_init();

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

    void loop()
    {
        HWSG_IOT_run.execute();
    }