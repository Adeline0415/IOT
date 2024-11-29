#include "Config.h"
#include <Arduino.h>

// 更新週期
const unsigned long UPDATE_PERIOD = 5000;

// DHT 配置
const int DHT_PIN = 4;
const int DHT_TYPE = 22;  // DHT22 的值定義為 22，與 DHT 庫一致

// 光敏
const int LDRPIN = 34;


// GPIO PIN 配置
const int PIN_LED = 2;
const int PIN_LED_R = 4;

// GPIO 初始狀態
const bool INITIAL_LED_STATE = LOW;
const bool INITIAL_LED_R_STATE = HIGH;

// 裝置相關配置
const char DEVICE_MAC[] = "00000000aaBB0D838";

// 訊息類型
const char LIGHT_MESSAGE_TYPE[] = "0265";
const char TEMPERATURE_MESSAGE_TYPE[] = "0267";
const char HUMIDITY_MESSAGE_TYPE[] = "0268";

// WiFi 設定
const char* WIFI_SSID = "shunelle";
const char* WIFI_PASSWORD = "0967020204";

// MQTT 設定
const char* MQTT_SERVER = "mqtt-pbl.kits.tw";
const int MQTT_PORT = 1883;
const char* MQTT_USER = "school";
const char* MQTT_PASSWORD = "nthu-hsnl";

// MQTT Topic 設定
const char* MQTT_UPLINK_TOPIC = "PBL/UL/";
const char* MQTT_DOWNLINK_TOPIC = "WAN-GW/DL/WetChicken";
