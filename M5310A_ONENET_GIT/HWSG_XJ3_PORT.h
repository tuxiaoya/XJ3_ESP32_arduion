#ifndef HWSG_XJ3_PORT.h
#define HWSG_XJ3_PORT .h

// define the port of XJ3  

// 定义三个硬件串口的波特率
#define DIWEN_UART_Baudrate     115200
#define HWSG_UART_Baudrate      1200
#define M5310A_UART_Baudrate    9600

// 定义三个硬件串口的 GPIO of  UART ： RXTX is ESP32 的 RXTX
#define M5310_RX3   3  //  origion lib is  rx1  tx3 ,because my faule to rhe wrong gpio  default is HardwareSerial 0
#define M5310_TX1   1  //  注意，引脚关系有点绕：驱动默认的TX0 对应 ESP32的引脚GPIO1 对应 DEV——BOARD板子的TX0插针 对应ESP32WROOM模组的35号孔
#define DIS_RX9     9    //  to diwen lcd diaplay ,  default is HardwareSerial 1
#define DIS_TX10    10  //  以此类推
#define HWSG_RX16   16 // to  hwsg uart  ,default is HardwareSerial 2
#define HWSG_TX17   17

// 定义键盘GPIO
#define KEY_UP            34
#define KEY_DOWN          33
#define KEY_LEFT          35
#define KEY_RIGHT         32
#define KEY_SELECT        27

// 定义 SD GPIO
#define SD_CLK 14
#define SD_D0 2
#define SD_D1 4
#define SD_D2 12
#define SD_D3 13
#define SD_CMD 15

// 定义 Spi GPIO
#define SPI_MOSI 23
#define SPI_MISO 22
#define SPI_CLK 5
#define SPI_RESET 25
#define SPI_IRQ 26

#endif