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
    M5310_Serial.begin(9600); // 闁跨喎鈧?鐔风到婵°倖绻堟慨鏍?顢″?楀牊瀚归梻鍥х墛閺嬬喖顢犻弮鎾村?规?佷焦鐗?閹凤拷 t
    // DIWEN_Serial.begin(115200, SERIAL_8N1, 17, 16);  //  rxPin, txPin
    HWSG_Serial.begin(1200, SERIAL_8N1, 18, 19); //  rxPin, txPin