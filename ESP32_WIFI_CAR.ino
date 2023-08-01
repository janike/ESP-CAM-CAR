//Viral Science www.youtube.com/c/viralscience  www.viralsciencecreativity.com
//ESP32 Camera Surveillance Car

#include "esp_camera.h"
#include "gSettings.h"
#include <WiFi.h>
#include <EEPROM.h>

// PIN Definitions for model
//#define CAMERA_MODEL_AI_THINKER

#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27

#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22


// GPIO Setting
int motorEnable = 12;  // RIGHT + BACK 13
int turn1 =  2;        // Left
int turn2 = 14;        // Right
int direction1 = 15;   // Back
int direction2 = 13;   // Front 12
int gpLed =  4;        // Light
struct s_settings g_settings;
unsigned long lastAction = 0;

// values
bool vt1 = LOW;
bool vt2 = LOW;
bool vd1 = LOW;
bool vd2 = LOW;

void setup() {
  pinMode(motorEnable, OUTPUT);
  digitalWrite(motorEnable, LOW);
  
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();

  pinMode(turn1, OUTPUT);
  pinMode(turn2, OUTPUT);
  pinMode(direction1, OUTPUT);
  pinMode(direction2, OUTPUT);
  pinMode(gpLed, OUTPUT);

  //initialize
  digitalWrite(turn1, LOW);
  digitalWrite(turn2, LOW);
  digitalWrite(direction1, LOW);
  digitalWrite(direction2, LOW);
  digitalWrite(gpLed, LOW);

  EEPROM.begin(512);

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  //init with high specs to pre-allocate larger buffers
  if(psramFound()){
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  //drop down frame size for higher initial frame rate
  sensor_t * s = esp_camera_sensor_get();
  s->set_framesize(s, FRAMESIZE_SVGA);

  EEPROMAnythingRead(EEPROM_START, reinterpret_cast<char*>(&g_settings), sizeof(g_settings));
  if(g_settings.c != 's') {
      loadDefaultSettings();
      EEPROMAnythingWrite(EEPROM_START, reinterpret_cast<char*>(&g_settings), sizeof(g_settings));
  }
  printSettings();

  if(g_settings.apMode == false && connectToWifi()) {
    ledBlink(2);
  } else {
    createWifi();
    ledBlink(3);
  }
  
  startCameraServer();

  Serial.print("Camera Ready! Use 'http://");
  Serial.print(WiFi.localIP());
  Serial.println("' to connect");

  digitalWrite(motorEnable, HIGH);
}

void loop()
{
  // stop car if no request for more than specified time
  if (lastAction && millis() - lastAction > ACTION_TIMEOUT) {
     //Serial.println(millis());
     lastAction = 0;
     Serial.println("Action timeout, stopping ...");
     WheelAct(LOW, LOW, LOW, LOW);
  }
  delay(5);
}

bool connectToWifi()
{
  int counter = 0;
  DEBUG_PRINT("Trying to connect to ");
  DEBUG_PRINTLN(g_settings.ssid);
  DEBUG_PRINT("With password ");
  DEBUG_PRINT(g_settings.passwd);
  WiFi.mode(WIFI_STA);
  WiFi.begin(g_settings.ssid, g_settings.passwd);
  
if(g_settings.staticIp) {
    while (WiFi.status() != WL_CONNECTED) {
      counter++;
      delay(500);
      DEBUG_PRINT(".");
      if(counter>15)
        return 0;
    }
    IPAddress ip;
    IPAddress gateway;
    IPAddress subnet;
    ip.fromString(g_settings.ip);
    gateway = WiFi.gatewayIP();
    subnet = WiFi.subnetMask();
    WiFi.disconnect();
    delay(10);
    WiFi.config(ip, gateway, subnet);
    WiFi.begin(g_settings.ssid, g_settings.passwd);
  }
  while (WiFi.status() != WL_CONNECTED) {
    counter++;
    delay(500);
    DEBUG_PRINT(".");
    if(counter>15)
      return 0;
  }
  DEBUG_PRINTLN();
  DEBUG_PRINT("Connected to ");
  DEBUG_PRINTLN(g_settings.ssid);
  DEBUG_PRINT("IP address: ");
  DEBUG_PRINTLN(WiFi.localIP());
  
  return 1;
};

bool createWifi()
{
  WiFi.disconnect(true); //prevents unreliable client connect (AP / STA colissions error)
  delay(50);
  DEBUG_PRINT("Creating wifi ");
  DEBUG_PRINTLN(AP_SSID);
  WiFi.softAP(AP_SSID, AP_PASS);
  delay(100);
  //WiFi.softAPConfig(ip, ip, subnet);

  //IPAddress ip = WiFi.softAPIP();
  DEBUG_PRINT("AP IP address: ");
  DEBUG_PRINTLN(WiFi.softAPIP());
};

void ledBlink(int count)
{
  for(int i=0; i<count;i++)
  {
    digitalWrite(gpLed, LED_HIGH);
    delay(BLINK_DELAY);
    digitalWrite(gpLed, LED_LOW);
    delay(BLINK_DELAY);
  }
}
