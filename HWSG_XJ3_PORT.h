// define the port of XJ3  




HardwareSerial M5310_Serial(0);
HardwareSerial DIWEN_Serial(1);
HardwareSerial HWSG_Serial(2);
bool ONENET_online = false;

//
//String txtMsg = "";                         // a string for incoming text
//unsigned int lastStringLength = txtMsg.length();     // previous length of the String

void setup()
{

    chipid = myesp32.getEfuseMac();                                  //The chip ID is essentially its MAC address(length: 6 bytes).
    Serial.printf("ESP32 Chip ID = %04X", (uint16_t)(chipid >> 32)); //print High 2 bytes
    Serial.printf("%08X\n", (uint32_t)chipid);                       //print Low 4bytes.

    // Open serial communications and wait for port to open:
    M5310_Serial.begin(9600); // 閿熷�?熷絽濡ゆ繈濮�?�?�堟嫹闂囧牊鏋熼鏃撴�?��?�佹�?鎷� t
    // DIWEN_Serial.begin(115200, SERIAL_8N1, 17, 16);  //  rxPin, txPin
    HWSG_Serial.begin(1200, SERIAL_8N1, 18, 19); //  rxPin, txPin