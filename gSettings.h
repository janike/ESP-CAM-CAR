
#define DEBUG
#ifdef DEBUG
 #define DEBUG_PRINT(x)  Serial.print (x)
 #define DEBUG_PRINTLN(x)  Serial.println (x)
#else
 #define DEBUG_PRINT(x)
 #define DEBUG_PRINTLN(x)
#endif

#define AP_SSID "ESP32_CAR"
#define AP_PASS "12345678"

#define EEPROM_START 10
#define BLINK_DELAY 200
#define LED_HIGH HIGH
#define LED_LOW LOW
#define ACTION_TIMEOUT 1000

struct s_settings {
  char c;
  char ssid[20];
  char passwd[15];
  bool staticIp;
  char ip[15];
  bool apMode;
};

void startCameraServer();
int EEPROMAnythingWrite(int pos, char *zeichen, int lenge);
int EEPROMAnythingRead(int pos, char *zeichen, int lenge);
void loadDefaultSettings();
void printSettings();
bool connectToWifi();
bool createWifi();
void WheelAct(int t1, int t2, int d1, int d2);
