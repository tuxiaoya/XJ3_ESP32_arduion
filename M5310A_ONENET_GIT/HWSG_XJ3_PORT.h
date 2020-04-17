#ifndef HWSG_XJ3_PORT.h
#define HWSG_XJ3_PORT .h

// define the port of XJ3  

// ��������Ӳ�����ڵĲ�����
#define DIWEN_UART_Baudrate     115200
#define HWSG_UART_Baudrate      1200
#define M5310A_UART_Baudrate    9600

// ��������Ӳ�����ڵ� GPIO of  UART �� RXTX is ESP32 �� RXTX
#define M5310_RX3   3  //  origion lib is  rx1  tx3 ,because my faule to rhe wrong gpio  default is HardwareSerial 0
#define M5310_TX1   1  //  ע�⣬���Ź�ϵ�е��ƣ�����Ĭ�ϵ�TX0 ��Ӧ ESP32������GPIO1 ��Ӧ DEV����BOARD���ӵ�TX0���� ��ӦESP32WROOMģ���35�ſ�
#define DIS_RX9     9    //  to diwen lcd diaplay ,  default is HardwareSerial 1
#define DIS_TX10    10  //  �Դ�����
#define HWSG_RX16   16 // to  hwsg uart  ,default is HardwareSerial 2
#define HWSG_TX17   17

// �������GPIO
#define KEY_UP            34
#define KEY_DOWN          33
#define KEY_LEFT          35
#define KEY_RIGHT         32
#define KEY_SELECT        27

// ���� SD GPIO
#define SD_CLK 14
#define SD_D0 2
#define SD_D1 4
#define SD_D2 12
#define SD_D3 13
#define SD_CMD 15

// ���� Spi GPIO
#define SPI_MOSI 23
#define SPI_MISO 22
#define SPI_CLK 5
#define SPI_RESET 25
#define SPI_IRQ 26

#endif