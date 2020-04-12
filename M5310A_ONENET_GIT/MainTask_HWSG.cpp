/** 
 *  TaskScheduler Test
 *
 *  Initially only tasks 1 and 2 are enabled
 *  Task1 runs every 2 seconds 10 times and then stops
 *  Task2 runs every 3 seconds indefinitely
 *  Task1 enables Task3 at its first run
 *  Task3 run every 5 seconds
 *  Task1 disables Task3 on its last iteration and changed Task2 to run every 1/2 seconds
 *  At the end Task2 is the only task running every 1/2 seconds
 */

#include <TaskScheduler.h>
#include <HardwareSerial.h>
#include "DIWEN_480.h"
#include <Ticker.h>
#include "MG_HWSG_2C.H"
#include "M5310A.H"

// Callback methods prototypes
void t1Callback();
void t2_HwsgMeasure_Callback(); //  every second read  uart HWSG 2C
void t3_NBIOT_Callback();       //   rvery sec send NB_IOT 

//Tasks
Task t4();
Task t1(2000, 10, &t1Callback);

Task TaskHwsgMeasure(1000, TASK_FOREVER, &t2_HwsgMeasure_Callback);
Task t3(5000, TASK_FOREVER, &t3Callback);

Scheduler runner;
NB_M5310A M5310A(0);                 // NB_M5310A  HardwareSerial 0
DIWEN480  DIWEN854480(1);              //  HardwareSerial uart Number 1
IR_Sensor_HWSG2C_Online HWSG2C_0(0, HWSG_TYPE_HIGHTEM ,2); // HWSG ID 0, HardwareSerial uart Number 2

void sys_init()
{
    DIWEN854480.Begin();    
    M5310A.init(); 
    HWSG2C_0.begin(); 
}

    void setup()
    {
        sys_init();
        Serial.begin(115200);
        Serial.println("Scheduler TEST");

        runner.init();
        Serial.println("Initialized scheduler");

        runner.addTask(t1);
        Serial.println("added t1");

        runner.addTask(t2);
        Serial.println("added t2");

        delay(5000);

        t1.enable();
        Serial.println("Enabled t1");
        t2.enable();
        Serial.println("Enabled t2");
    }

    void t1Callback()
    {
        Serial.print("t1: ");
        Serial.println(millis());

        if (t1.isFirstIteration())
        {
            runner.addTask(t3);
            t3.enable();
            Serial.println("t1: enabled t3 and added to the chain");
        }

        if (t1.isLastIteration())
        {
            t3.disable();
            runner.deleteTask(t3);
            t2.setInterval(500);
            Serial.println("t1: disable t3 and delete it from the chain. t2 interval set to 500");
        }
    }

    void t2_HwsgMeasure_Callback()
    {
        Serial.print("t2: ");
        Serial.println(millis());
    }

    void t3Callback()
    {
        Serial.print("t3: ");
        Serial.println(millis());
    }

    void loop()
    {
        runner.execute();
    }